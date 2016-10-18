// + ======================================== +
// |         gl-oo-interface library          |
// |            Module: GLOO Mesh.            |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +

// ============================================================================================ //
// Group
// ============================================================================================ //
// 3D-surface meshes are made up by groups, which are parts that share the same
// materials or textures.
//
// Each group has a single vertex buffer in GPU, which can follow two kinds of storage:
// 1. Interleaved (Tighly Packed):  (P N T) (P N T) ... (P N T)
// 2. Batched (Sub-Buffered):       (P P ... P) (N N ... N) (T T ... T)
// Where P is the vertex positions array, N is the vertex normals array and so on.
//
// The storage format is provided by the template parameter Storage Format. 
//
// Usage:
// 
// TODO(Castiel).
// ...
//
//
// IMPORTANT:
// If you want to use a group in different shader programs, you should 
// make sure that all of them have the same attribute location (typically,
// loc=0 for position, loc=1 for normal and so on).
//
// ============================================================================================ //

#pragma once

#include "gl_header.h"
#include <vector>
#include <initializer_list>

namespace gloo
{

// StorageFormat specifies how geometry data is stored into the GPU buffer.
enum StorageFormat
{
  Interleave,  // (Tighly Packed):  (P N T) (P N T) ... (P N T)
  Batch,       // (Sub-Buffered):   (P P ... P) (N N ... N) (T T ... T)
};

// VertexAttribute specifies properties of a per-vertex data such as position or normal.
struct VertexAttribute
{
  GLuint mSize;      // Dimensionality of data (position = 3d, uv = 2d, normal = 3d).
  GLuint mLoc;       // GL Attribute Location (usually provided by gloo::Renderer).
};

template <StorageFormat F>
class MeshGroup
{
public:
  /* Most used methods */
  // TODO: add options for draw mode and buffer data usage.
  MeshGroup(std::initializer_list<VertexAttribute> vertexAttributeList,
            int numVertices, int numElements);

  ~MeshGroup();

  // TODO: document.
  bool Load(const std::vector<GLfloat*> & bufferList, const GLuint* indices, GLenum drawMode);
  bool Load(const GLfloat* buffer, const GLuint* indices, GLenum drawMode);

  // Should be called on display function (it calls glDrawElements).
  void Render() const;

  // Generate buffers on GPU (VAO, VBO, EAB).
  void GenerateBuffers(const GLfloat* vertices, const GLuint* elements);

  // Destroys buffers on GPU (VAO, VBO, EAB).
  void ClearBuffers();

private:
  // Specifies vertex attribute object (how attributes are spatially stored into VBO and
  // mapped to attribute locations on shader).
  void BuildVAO();

  /* Attributes */

  // OpenGL buffer IDs.
  GLuint mEab { 0 };  // Element array buffer.
  GLuint mVao { 0 };  // Vertex attribute object.
  GLuint mVbo { 0 };  // Vertex buffer object.

  // Mesh attributes.
  GLenum mDrawMode { GL_TRIANGLES };  // How mesh is rendered (drawing mode).
  GLuint mVertexSize;   // Number of floating points stored per vertex.
  GLuint mNumVertices;  // Number of vertices in this group.
  GLuint mNumElements;  // Number of elements (of indices of vertex).

  // Vertex attributes descriptor -> specifies which attributes a vertex contain and also
  // their dimensionality and order. This is constant within the lifetime of a MeshGroup.
  std::vector<VertexAttribute> mVertexAttributeList;
};

// ============================================================================================ //
// Implementation of template functions.

/* Constructor */
template <StorageFormat F>
MeshGroup<F>::MeshGroup(std::initializer_list<VertexAttribute> vertexAttributeList, 
                        int numVertices, int numElements)
: mVertexAttributeList(vertexAttributeList)
, mNumVertices(numVertices)
, mNumElements(numElements)
{ 
  mVertexSize = 0;
  for (auto & attribute : mVertexAttributeList) 
  {
    mVertexSize += attribute.mSize;
  }
}

/* Destructor */
template <StorageFormat F>
MeshGroup<F>::~MeshGroup() 
{
  MeshGroup<F>::ClearBuffers();
}

/* Rendering method */
template <StorageFormat F>
void MeshGroup<F>::Render() const
{
  glBindVertexArray(mVao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEab);

  glDrawElements(
    mDrawMode,         // mode (GL_LINES, GL_TRIANGLES, ...)
    mNumElements,      // number of vertices.
    GL_UNSIGNED_INT,   // type.
    (void*)0           // element array buffer offset.
   );
}

/* Generate buffers */
template <StorageFormat F>
void MeshGroup<F>::GenerateBuffers(const GLfloat* vertices, const GLuint* elements)
{
  // Generate Buffers.
  glGenVertexArrays(1, &mVao);  // Vertex array object.
  glGenBuffers(1, &mVbo);       // Vertex buffer object.
  glGenBuffers(1, &mEab);       // Element array buffer.

  // Allocate buffer for elements (EAB).
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEab);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumElements * sizeof(GLuint),
               elements, GL_STATIC_DRAW);

  // Allocate buffer for vertices (VBO).
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glBufferData(GL_ARRAY_BUFFER, mVertexSize * mNumVertices * sizeof(GLfloat),
               vertices, GL_STATIC_DRAW);
}

/* Delete buffers */
template <StorageFormat F>
void MeshGroup<F>::ClearBuffers()
{
  glDeleteBuffers(1, &mVbo);
  glDeleteBuffers(1, &mEab);
  glDeleteVertexArrays(1, &mVao);
}

// ============================================================================================= //
// Specializations for different StorageFormats.

// ================ Batched Storage =================== // 

template <>
bool MeshGroup<Interleave>::Load(
    const std::vector<GLfloat*> & bufferList, const GLuint* indices, GLenum drawMode
);

template <>
bool MeshGroup<Batch>::Load(
    const std::vector<GLfloat*> & bufferList, const GLuint* indices, GLenum drawMode
);

template <>
void MeshGroup<Interleave>::BuildVAO();


// ================ Interleaved Storage ==================== // 

template <>
bool MeshGroup<Interleave>::Load(
    const GLfloat* buffer, const GLuint* indices, GLenum drawMode
);

template <>
bool MeshGroup<Batch>::Load(
    const GLfloat* buffer, const GLuint* indices, GLenum drawMode
);

template <>
void MeshGroup<Batch>::BuildVAO();

}  // namespace gloo.