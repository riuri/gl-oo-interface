#include "group.h"

namespace gloo
{

template <>
bool MeshGroup<Interleave>::Load(
    const std::vector<GLfloat*> & bufferList, const GLuint* indices, GLenum drawMode)
{
  mDrawMode = drawMode;

  // Create temporary buffers to transfer geometry and elements to GPU.
  std::vector<GLfloat> vertexBuffer;
  vertexBuffer.reserve(mNumVertices * mVertexSize);

  // Copy geometry to temporary buffers in memory.
  for (int i = 0; i < mNumVertices; i++)
  {
    // For each attribute,
    // for (auto & attrib : mVertexAttributeList)
    for (int j = 0; j < mVertexAttributeList.size(); j++)
    {
      auto & attrib = mVertexAttributeList[j];
      const int size      = attrib.mSize;
      const float* buffer = bufferList[j];
      const GLuint loc    = attrib.mLoc;

      if ((size > 0) && (buffer != nullptr))
      {
        // Add elements (coordinate, normals, ...).
        for (int k = 0; k < size; k++) 
        {
          vertexBuffer.push_back(buffer[size*i + k]);
        }
      }
    }
  }

  // Reserve vertex buffer and initialize element array (indices array).
  if (indices)  // Element array provided.
  {
    MeshGroup<Interleave>::GenerateBuffers(vertexBuffer.data(), indices);
  }
  else  // Element array wasn't provided -- build it up.
  {
    std::vector<GLuint> elementsBuffer(mNumElements);

    for (int i = 0; i < mNumElements; i++)
      elementsBuffer[i] = i;

    MeshGroup<Interleave>::GenerateBuffers(vertexBuffer.data(), elementsBuffer.data());
  }

  MeshGroup<Interleave>::BuildVAO();
}

template <>
bool MeshGroup<Interleave>::Load(
    const GLfloat* buffer, const GLuint* indices, GLenum drawMode) 
{
  mDrawMode = drawMode;

  // Reserve vertex buffer and initialize element array (indices array).
  if (indices)  // Element array provided.
  {
    MeshGroup<Interleave>::GenerateBuffers(buffer, indices);
  }
  else  // Element array wasn't provided -- build it up.
  {
    std::vector<GLuint> elementsBuffer(mNumElements);

    for (int i = 0; i < mNumElements; i++)
      elementsBuffer[i] = i;

    MeshGroup<Interleave>::GenerateBuffers(buffer, elementsBuffer.data());
  }

  MeshGroup<Interleave>::BuildVAO();
}

template <>
void MeshGroup<Interleave>::BuildVAO()
{
  // Specify VAO.
  glBindVertexArray(mVao);
  int offset = 0;
  GLfloat stride = sizeof(GLfloat) * mVertexSize;
  for (auto & attrib : mVertexAttributeList)
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
bool MeshGroup<Batch>::Load(
    const std::vector<GLfloat*> & bufferList, const GLuint* indices, GLenum drawMode)
{
  mDrawMode = drawMode;

  // Reserve vertex buffer and initialize element array (indices array).
  if (indices)  // Element array provided.
  {
    MeshGroup<Batch>::GenerateBuffers(nullptr, indices);
  }
  else  // Element array wasn't provided -- build it up.
  {
    std::vector<GLuint> elementsBuffer(mNumElements);

    for (int i = 0; i < mNumElements; i++)
      elementsBuffer[i] = i;

    MeshGroup<Batch>::GenerateBuffers(nullptr, elementsBuffer.data());
  }

  MeshGroup<Batch>::BuildVAO();

  // Upload subdata of geometry to GPU.
  int offset = 0;
  for (int j = 0; j < mVertexAttributeList.size(); j++)
  {
    auto & attrib = mVertexAttributeList[j];
    const int size      = attrib.mSize;
    const float* buffer = bufferList[j];
    const GLuint loc    = attrib.mLoc;

    if ((size > 0) && (buffer != nullptr))
    {
      glBufferSubData(GL_ARRAY_BUFFER, offset, size*mNumVertices*sizeof(GLfloat), buffer);
    }

    offset += size*mNumVertices * sizeof(GLfloat);
  }
}

template <>
bool MeshGroup<Batch>::Load(
    const GLfloat* buffer, const GLuint* indices, GLenum drawMode)
{
  mDrawMode = drawMode;

  // Initialize element array (indices array).
  if (indices)  // Element array provided.
  {
    MeshGroup<Batch>::GenerateBuffers(buffer, indices);
  }
  else  // Element array wasn't provided -- build it up.
  {
    std::vector<GLuint> elementsBuffer(mNumElements);

    for (int i = 0; i < mNumElements; i++)
      elementsBuffer[i] = i;

    MeshGroup<Batch>::GenerateBuffers(buffer, elementsBuffer.data());
  }

  MeshGroup<Batch>::BuildVAO();
}

template <>
void MeshGroup<Batch>::BuildVAO()
{
  // Specify VAO.
  glBindVertexArray(mVao);
  int offset = 0;
  for (auto & attrib : mVertexAttributeList)
  {
    const int size      = attrib.mSize;
    const GLuint loc    = attrib.mLoc;

    if (size > 0)
    {
      glEnableVertexAttribArray(loc);

      // Specify internal storage architecture of Vertex Buffer.
      glVertexAttribPointer(loc, size, GL_FLOAT, GL_FALSE, 0, 
        (void*)(sizeof(GLfloat) * offset*mNumVertices));
    }
    else 
    {
      glDisableVertexAttribArray(loc);
    }

    offset += size;
  }
}

}  // namespace gloo.