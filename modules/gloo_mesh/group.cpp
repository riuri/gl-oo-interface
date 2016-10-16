#include "group.h"

namespace gloo
{

template <>
bool StaticGroup<Interleave>::Load(
    const std::vector<GLfloat*>        & bufferList,
    const std::vector<VertexAttribute> & vertexAttributeList,
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
  bufferVertices.reserve(numVertices * vertexSize);

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

  // Upload indices to GPU.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEab);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), 
               bufferIndices.data(), GL_STATIC_DRAW);

  // Copy geometry to temporary buffers.
  for (int i = 0; i < numVertices; i++)
  {
    // For each attribute,
    // for (auto & attrib : vertexAttributeList)
    for (int j = 0; j < vertexAttributeList.size(); j++)
    {
      auto & attrib = vertexAttributeList[j];
      const int size      = attrib.mSize;
      const float* buffer = bufferList[j];
      const GLuint loc    = attrib.mLoc;

      if ((size > 0) && (buffer != nullptr))
      {
        // Add elements (coordinate, normals, ...).
        for (int k = 0; k < size; k++) 
        {
          bufferVertices.push_back(buffer[size*i + k]);
        }
      }
    }
  }

  // Upload vertices to GPU.
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glBufferData(GL_ARRAY_BUFFER, vertexSize * numVertices * sizeof(GLfloat),
               bufferVertices.data(), GL_STATIC_DRAW);

  StaticGroup<Interleave>::SpecifyVAO(vertexAttributeList, vertexSize, numVertices);
}

template <>
bool StaticGroup<Interleave>::Load(
    const GLfloat* buffer,
    const std::vector<VertexAttribute> & vertexAttributeList,
    const GLuint* indices, int numVertices, int numIndices, GLenum drawMode) 
{
  mDrawMode = drawMode;
  mNumIndices = numIndices;

  // Compute vertex size by adding up all attribute sizes.
  int vertexSize = 0;
  for (auto& attrib : vertexAttributeList) 
    vertexSize += attrib.mSize; 

  // Create temporary buffers for transfering geometry and elements to GPU.
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

  // Upload indices to GPU.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEab);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), 
               bufferIndices.data(), GL_STATIC_DRAW);

 
  // Upload vertices to GPU.
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glBufferData(GL_ARRAY_BUFFER, vertexSize * numVertices * sizeof(GLfloat),
               buffer, GL_STATIC_DRAW);

  StaticGroup<Interleave>::SpecifyVAO(vertexAttributeList, vertexSize, numVertices);
}

template <>
void StaticGroup<Interleave>::SpecifyVAO(const std::vector<VertexAttribute> & vertexAttributeList, 
    int vertexSize, int numVertices)
{
  // Specify VAO.
  glBindVertexArray(mVao);
  int offset = 0;
  GLfloat stride = sizeof(GLfloat) * vertexSize;
  for (auto & attrib : vertexAttributeList)
  {
    const int size      = attrib.mSize;
    const GLuint loc    = attrib.mLoc;

    if (size > 0)
    {
      glEnableVertexAttribArray(loc);

      // Specify internal storage architecture of Vertex Buffer.
      glVertexAttribPointer(loc, size, GL_FLOAT, GL_FALSE, stride, 
        (void*)(sizeof(GLfloat) * offset));
    }
    else 
    {
      glDisableVertexAttribArray(loc);
    }

    offset += size;
  }
}

// ============================================================================================= //

template <>
bool StaticGroup<Batch>::Load(
    const std::vector<GLfloat*>        & bufferList,
    const std::vector<VertexAttribute> & vertexAttributeList,
    const GLuint* indices, int numVertices, int numIndices, GLenum drawMode)
{
  mDrawMode = drawMode;
  mNumIndices = numIndices;

  // Compute vertex size by adding up all attribute sizes.
  int vertexSize = 0;
  for (auto& attrib : vertexAttributeList) 
    vertexSize += attrib.mSize; 

  // Create temporary buffers for transfering geometry and elements to GPU.
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

  // Upload indices to GPU.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEab);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), 
               bufferIndices.data(), GL_STATIC_DRAW);

  // Reserve buffer on GPU.
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glBufferData(GL_ARRAY_BUFFER, vertexSize * numVertices * sizeof(GLfloat),
               nullptr, GL_STATIC_DRAW);

  // Upload subdata of geometry to GPU.
  int offset = 0;
  for (int j = 0; j < vertexAttributeList.size(); j++)
  {
    auto & attrib = vertexAttributeList[j];
    const int size      = attrib.mSize;
    const float* buffer = bufferList[j];
    const GLuint loc    = attrib.mLoc;

    if ((size > 0) && (buffer != nullptr))
    {
      glBufferSubData(GL_ARRAY_BUFFER, offset, size*numVertices*sizeof(GLfloat), buffer);
    }

    offset += size*numVertices * sizeof(GLfloat);
  }

  StaticGroup<Batch>::SpecifyVAO(vertexAttributeList, vertexSize, numVertices);
}

template <>
bool StaticGroup<Batch>::Load(
    const GLfloat* buffer,
    const std::vector<VertexAttribute> & vertexAttributeList,
    const GLuint* indices, int numVertices, int numIndices, GLenum drawMode)
{
  mDrawMode = drawMode;
  mNumIndices = numIndices;

  // Compute vertex size by adding up all attribute sizes.
  int vertexSize = 0;
  for (auto& attrib : vertexAttributeList) 
    vertexSize += attrib.mSize; 

  // Create temporary buffers for transfering geometry and elements to GPU.
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

  // Upload indices to GPU.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEab);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), 
               bufferIndices.data(), GL_STATIC_DRAW);

  // Upload entire buffer to GPU.
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glBufferData(GL_ARRAY_BUFFER, vertexSize * numVertices * sizeof(GLfloat),
               buffer, GL_STATIC_DRAW);


  StaticGroup<Batch>::SpecifyVAO(vertexAttributeList, vertexSize, numVertices);
}

template <>
void StaticGroup<Batch>::SpecifyVAO(const std::vector<VertexAttribute> & vertexAttributeList, 
    int vertexSize, int numVertices)
{
  // Specify VAO.
  glBindVertexArray(mVao);
  int offset = 0;
  for (int j = 0; j < vertexAttributeList.size(); j++)
  {
    auto & attrib = vertexAttributeList[j];
    const int size      = attrib.mSize;
    const GLuint loc    = attrib.mLoc;

    if (size > 0)
    {
      glEnableVertexAttribArray(loc);

      // Specify internal storage architecture of Vertex Buffer.
      glVertexAttribPointer(loc, size, GL_FLOAT, GL_FALSE, 0, 
        (void*)(sizeof(GLfloat) * offset*numVertices));
    }
    else 
    {
      glDisableVertexAttribArray(loc);
    }

    offset += size;
  }
}

}  // namespace gloo.