#include "group.h"

namespace gloo
{

template <>
bool StaticGroup<Interleave>::Load(const std::vector<VertexAttribute> & vertexAttributeList,
                                   const GLuint* indices, int numVertices, int numIndices, GLenum drawMode)
{
  mDrawMode = drawMode;
  mNumIndices = numIndices;

  // Compute vertex size by adding up all attribute sizes.
  int vertexSize = 0;
  for (auto& attrib : vertexAttributeList) 
    vertexSize += attrib.mSize; 

  // Create temporary buffers for transfering geometry and elements to GPU.
  std::vector<GLfloat> bufferVertices;
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
  
  // For each vertex,
  for (int i = 0; i < numVertices; i++)
  {
    // For each attribute,
    for (auto & attrib : vertexAttributeList)
    {
      const int size      = attrib.mSize;
      const float* buffer = attrib.mBuffer;
      const GLuint loc    = attrib.mLoc;

      if ((size > 0) && (buffer != nullptr))
      {
        glEnableVertexAttribArray(loc);

        // Add elements (coordinate, normals, ...).
        for (int k = 0; k < size; k++) 
        {
          bufferVertices.push_back(buffer[size*i + k]);
        }
      }
      else 
      {
        glDisableVertexAttribArray(loc);
      }
    }
  }

  // Upload vertices to GPU.
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glBufferData(GL_ARRAY_BUFFER, vertexSize * numVertices * sizeof(GLfloat),
               bufferVertices.data(), GL_STATIC_DRAW);

  int startIndex = 0;
  GLfloat stride = sizeof(GLfloat) * vertexSize;
  for (auto & attrib: vertexAttributeList)
  { 
    const int size      = attrib.mSize;
    const float* buffer = attrib.mBuffer;
    const GLuint loc    = attrib.mLoc;

    // Specify internal storage architecture of Vertex Buffer.
    glVertexAttribPointer(loc, size, GL_FLOAT, GL_FALSE, stride, 
        (void*)(sizeof(GLfloat) * startIndex));

    startIndex += size;
  }
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