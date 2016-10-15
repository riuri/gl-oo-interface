#include "group.h"

namespace gloo
{

template <>
bool StaticGroup<Interleave>::Load(GLuint programHandle,
                                   const GLfloat* positions, 
                                   const GLfloat* colors,
                                   const GLfloat* normals,
                                   const GLfloat* uv,
                                   const GLuint* indices,
                                   int numVertices,
                                   int numIndices,
                                   GLenum drawMode)
{
  mDrawMode = drawMode;



}

template <>
bool StaticGroup<Interleave>::Load(const std::vector<VertexAttribute> & vertexAttributeList,
                                   const GLuint* indices, int numVertices, int numIndices, GLenum drawMode)
{

}

template <>
bool StaticGroup<Batch>::Load(GLuint programHandle,
                              const GLfloat* positions, 
                              const GLfloat* colors,
                              const GLfloat* normals,
                              const GLfloat* uv,
                              const GLuint* indices,
                              int numVertices,
                              int numIndices,
                              GLenum drawMode)
{
  mDrawMode = drawMode;
  mNumIndices = numIndices;

  // Initialize format flags.
  bool hasColors  = (colors  != nullptr);
  bool hasNormals = (normals != nullptr);
  bool hasUV      = (uv      != nullptr);
  int vertexSize = 3 + 3*hasColors + 3*hasNormals + 2*hasUV;

  // Create temporary buffers for transfering geometry and elements to GPU.
  std::vector<GLfloat> bufferVertices(vertexSize * numVertices);
  std::vector<GLuint> bufferIndices(numIndices);

  // Copy geometry and elements to temporary buffers.
  GLfloat* dest = bufferVertices.data();
  memcpy(dest, positions, 3*sizeof(GLfloat)*numVertices);
  dest += 3*numVertices;
  if (hasColors)
  {
    memcpy(dest, colors, 3*sizeof(GLfloat)*numVertices);
    dest += 3*numVertices;
  }
  if (hasNormals)
  {
    memcpy(dest, normals, 3*sizeof(GLfloat)*numVertices);
    dest += 3*numVertices;
  }
  if (hasUV)
  {
    memcpy(dest, uv, 2*sizeof(GLfloat)*numVertices);
    dest += 2*numVertices;
  }

  // Initialize element array (indices array).
  if (indices)  // Element array provided.
  {
    memcpy(bufferIndices.data(), indices, sizeof(GLuint)*numIndices);
  }
  else  // Element array wasn't provided -- build it up.
  {
    for (int i = 0; i < numIndices; i++)
    {
      bufferIndices[i] = i;
    }
  }

  GLuint locTexCoordAttrib = glGetAttribLocation(programHandle, "in_uv");
  GLuint locPositionAttrib = glGetAttribLocation(programHandle, "in_position");
  GLuint locNormalAttrib   = glGetAttribLocation(programHandle, "in_normal");
  GLuint locColorAttrib    = glGetAttribLocation(programHandle, "in_color");

  // Generate Buffers.
  glGenVertexArrays(1, &mVao);  // Vertex array object.
  glGenBuffers(1, &mVbo);       // Vertex buffer object.
  glGenBuffers(1, &mEab);       // Element array buffer.

  // Specify VAO.
  glBindVertexArray(mVao);

  // Upload indices to GPU.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEab);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), 
               bufferIndices.data(), GL_STATIC_DRAW);

  // Enable/Disable each vertex attributes.
  glEnableVertexAttribArray(locPositionAttrib);

  if (hasColors)
    glEnableVertexAttribArray(locColorAttrib);
  else
    glDisableVertexAttribArray(locColorAttrib);
  
  if (hasNormals)
    glEnableVertexAttribArray(locNormalAttrib);
  else
    glDisableVertexAttribArray(locNormalAttrib);

  if (hasUV)
    glEnableVertexAttribArray(locTexCoordAttrib);
  else
    glDisableVertexAttribArray(locTexCoordAttrib);

  // Upload vertices to GPU.
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glBufferData(GL_ARRAY_BUFFER, vertexSize * numVertices * sizeof(GLfloat), 
               bufferVertices.data(), GL_STATIC_DRAW);

  // Specify internal storage of Vertex Buffer.
  glVertexAttribPointer(locPositionAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glVertexAttribPointer(locColorAttrib,    3, GL_FLOAT, GL_FALSE, 0, 
      (void*)(sizeof(GLfloat) * 3*numVertices));

  glVertexAttribPointer(locNormalAttrib,   3, GL_FLOAT, GL_FALSE, 0, 
      (void*)(sizeof(GLfloat) * (3 + 3*hasColors)*numVertices));

  glVertexAttribPointer(locTexCoordAttrib, 2, GL_FLOAT, GL_FALSE, 0, 
      (void*)(sizeof(GLfloat) * (3 + 3*hasColors + 3*hasNormals)*numVertices));
}

template <>
bool StaticGroup<Batch>::Load(const std::vector<VertexAttribute> & vertexAttributeList,
                              const GLuint* indices, int numVertices, int numIndices, GLenum drawMode)
{
  mDrawMode = drawMode;
  mNumIndices = numIndices;

  // Compute vertex size by adding up all attribute sizes.
  int vertexSize = 0;
  for (auto& attrib : vertexAttributeList) 
    vertexSize += attrib.mSize; 

  // Create temporary buffers for transfering geometry and elements to GPU.
  std::vector<GLfloat> bufferVertices(vertexSize * numVertices);
  std::vector<GLuint> bufferIndices(numIndices);  

  // Initialize element array (indices array).
  if (indices)  // Element array provided.
  {
    memcpy(bufferIndices.data(), indices, sizeof(GLuint)*numIndices);
  }
  else  // Element array wasn't provided -- build it up.
  {
    for (int i = 0; i < numIndices; i++)
    {
      bufferIndices[i] = i;
    }
  }

  // Generate Buffers.
  glGenVertexArrays(1, &mVao);  // Vertex array object.
  glGenBuffers(1, &mVbo);       // Vertex buffer object.
  glGenBuffers(1, &mEab);       // Element array buffer.

  // Specify VAO.
  glBindVertexArray(mVao);


  // Upload indices to GPU.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEab);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), 
               bufferIndices.data(), GL_STATIC_DRAW);

  // Copy geometry and elements to temporary buffers.
  GLfloat* dest = bufferVertices.data();
  for (auto & attrib : vertexAttributeList)
  {
    const int size      = attrib.mSize;
    const float* buffer = attrib.mBuffer;
    const GLuint loc    = attrib.mLoc;

    if ((size > 0) && (buffer != nullptr))
    {
      glEnableVertexAttribArray(loc);
      memcpy(dest, buffer, size*sizeof(GLfloat)*numVertices);
      dest += size*numVertices;
    }
    else 
    {
      glDisableVertexAttribArray(loc);
    }
  }

  // Upload vertices to GPU.
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glBufferData(GL_ARRAY_BUFFER, vertexSize * numVertices * sizeof(GLfloat), 
               bufferVertices.data(), GL_STATIC_DRAW);

  int startIndex = 0;
  for (auto & attrib: vertexAttributeList)
  { 
    const int size      = attrib.mSize;
    const float* buffer = attrib.mBuffer;
    const GLuint loc    = attrib.mLoc;

    // Specify internal storage architecture of Vertex Buffer.
    glVertexAttribPointer(loc, size, GL_FLOAT, GL_FALSE, 0, 
        (void*)(sizeof(GLfloat) * startIndex*numVertices));

    startIndex += size;
  }
}


}  // namespace gloo.