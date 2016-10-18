#include "group.h"

namespace gloo
{

template <>
bool MeshGroup<Interleave>::Load(
    const std::vector<GLfloat*> & bufferList, const GLuint* indices, GLenum drawMode) 
{
  mDrawMode = drawMode;

  // Create temporary buffers for transfering geometry and elements to GPU.
  std::vector<GLfloat> bufferVertices;
  std::vector<GLuint> bufferIndices(mNumElements);
  bufferVertices.reserve(mNumVertices * mVertexSize);

  // Initialize element array (indices array).
  if (indices)  // Element array provided.
  {
    memcpy(bufferIndices.data(), indices, sizeof(GLuint)*mNumElements);
  }
  else  // Element array wasn't provided -- build it up.
  {
    for (int i = 0; i < mNumElements; i++)
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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumElements * sizeof(GLuint), 
               bufferIndices.data(), GL_STATIC_DRAW);

  // Copy geometry to temporary buffers.
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
          bufferVertices.push_back(buffer[size*i + k]);
        }
      }
    }
  }

  // Upload vertices to GPU.
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glBufferData(GL_ARRAY_BUFFER, mVertexSize * mNumVertices * sizeof(GLfloat),
               bufferVertices.data(), GL_STATIC_DRAW);

  MeshGroup<Interleave>::BuildVAO();
}

template <>
bool MeshGroup<Interleave>::Load(
    const GLfloat* buffer, const GLuint* indices, GLenum drawMode) 
{
  mDrawMode = drawMode;

  // Create temporary buffers for transfering geometry and elements to GPU.
  std::vector<GLuint> bufferIndices(mNumElements);

  // Initialize element array (indices array).
  if (indices)  // Element array provided.
  {
    memcpy(bufferIndices.data(), indices, sizeof(GLuint)*mNumElements);
  }
  else  // Element array wasn't provided -- build it up.
  {
    for (int i = 0; i < mNumElements; i++)
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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumElements * sizeof(GLuint), 
               bufferIndices.data(), GL_STATIC_DRAW);

 
  // Upload vertices to GPU.
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glBufferData(GL_ARRAY_BUFFER, mVertexSize * mNumVertices * sizeof(GLfloat),
               buffer, GL_STATIC_DRAW);

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

  // Create temporary buffers for transfering geometry and elements to GPU.
  std::vector<GLuint> bufferIndices(mNumElements);

  // Initialize element array (indices array).
  if (indices)  // Element array provided.
  {
    memcpy(bufferIndices.data(), indices, sizeof(GLuint)*mNumElements);
  }
  else  // Element array wasn't provided -- build it up.
  {
    for (int i = 0; i < mNumElements; i++)
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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumElements * sizeof(GLuint), 
               bufferIndices.data(), GL_STATIC_DRAW);

  // Reserve buffer on GPU.
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glBufferData(GL_ARRAY_BUFFER, mVertexSize * mNumVertices * sizeof(GLfloat),
               nullptr, GL_STATIC_DRAW);

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

  MeshGroup<Batch>::BuildVAO();
}

template <>
bool MeshGroup<Batch>::Load(
    const GLfloat* buffer, const GLuint* indices, GLenum drawMode)
{
  mDrawMode = drawMode;

  // Create temporary buffers for transfering geometry and elements to GPU.
  std::vector<GLuint> bufferIndices(mNumElements);

  // Initialize element array (indices array).
  if (indices)  // Element array provided.
  {
    memcpy(bufferIndices.data(), indices, sizeof(GLuint)*mNumElements);
  }
  else  // Element array wasn't provided -- build it up.
  {
    for (int i = 0; i < mNumElements; i++)
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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumElements * sizeof(GLuint), 
               bufferIndices.data(), GL_STATIC_DRAW);

  // Upload entire buffer to GPU.
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glBufferData(GL_ARRAY_BUFFER, mVertexSize * mNumVertices * sizeof(GLfloat),
               buffer, GL_STATIC_DRAW);


  MeshGroup<Batch>::BuildVAO();
}

template <>
void MeshGroup<Batch>::BuildVAO()
{
  // Specify VAO.
  glBindVertexArray(mVao);
  int offset = 0;
  for (int j = 0; j < mVertexAttributeList.size(); j++)
  {
    auto & attrib = mVertexAttributeList[j];
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