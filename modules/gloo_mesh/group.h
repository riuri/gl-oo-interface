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
// StaticGroup<Batch>* meshGroup = new StaticGroup<Batch>();
// meshGroup->Load({ {3, colAttribLoc,  squareColors},
//                   {3, posAttribLoc,  squareVertices}},
//                    nullptr, 4, 4, GL_TRIANGLES);
// ...
// meshGroup->Render();
//
// meshGroup->Clear();
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
class StaticGroup
{
public:
  StaticGroup()  { }
  ~StaticGroup();

  // TODO: document.
  bool Load(const std::vector<GLfloat*>        & bufferList,
            const std::vector<VertexAttribute> & vertexAttributeList,
            const GLuint* indices, int numVertices, int numIndices, GLenum drawMode);


  bool Load(const GLfloat* buffer,
            const std::vector<VertexAttribute> & vertexAttributeList,
            const GLuint* indices, int numVertices, int numIndices, GLenum drawMode);

  // Should be called on display function (it calls glDrawElements).
  void Render() const;

  // Destroys buffers on GPU (VAO, VBO, EAB).
  void Clear();

  // Specifies vertex attribute object (how attributes are spatially stored into VBO and
  // mapped to attribute locations on shader).
  void SpecifyVAO(const std::vector<VertexAttribute> & vertexAttributeList, 
    int vertexSize, int numVertices);

private:
  // OpenGL buffer IDs.
  GLuint mEab { 0 };
  GLuint mVao { 0 };
  GLuint mVbo { 0 };

  // Geometry and rendering options.
  GLuint mNumIndices { 0 };
  GLenum mDrawMode { GL_TRIANGLES };
};


// ============================================================================================= //
// Specializations for different StorageFormats.

template <>
bool StaticGroup<Interleave>::Load(
    const std::vector<GLfloat*>        & bufferList,
    const std::vector<VertexAttribute> & vertexAttributeList,
    const GLuint* indices, int numVertices, int numIndices, GLenum drawMode
);

template <>
bool StaticGroup<Batch>::Load(
    const std::vector<GLfloat*>        & bufferList,
    const std::vector<VertexAttribute> & vertexAttributeList,
    const GLuint* indices, int numVertices, int numIndices, GLenum drawMode
);

template <>
void StaticGroup<Interleave>::SpecifyVAO(const std::vector<VertexAttribute> & vertexAttributeList, 
    int vertexSize, int numVertices);


// ================

template <>
bool StaticGroup<Interleave>::Load(
    const GLfloat* buffer,
    const std::vector<VertexAttribute> & vertexAttributeList,
    const GLuint* indices, int numVertices, int numIndices, GLenum drawMode
);

template <>
bool StaticGroup<Batch>::Load(
    const GLfloat* buffer,
    const std::vector<VertexAttribute> & vertexAttributeList,
    const GLuint* indices, int numVertices, int numIndices, GLenum drawMode
);

template <>
void StaticGroup<Batch>::SpecifyVAO(const std::vector<VertexAttribute> & vertexAttributeList, 
    int vertexSize, int numVertices);

// ============================================================================================ //
// Implementation of template functions.


template <StorageFormat F>
StaticGroup<F>::~StaticGroup() 
{
  StaticGroup<F>::Clear();
}

template <StorageFormat F>
void StaticGroup<F>::Render() const
{
  glBindVertexArray(mVao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEab);

  glDrawElements(
   mDrawMode,         // mode (GL_LINES, GL_TRIANGLES, ...)
   mNumIndices,       // number of vertices.
   GL_UNSIGNED_INT,   // type.
   (void*)0           // element array buffer offset.
  );
}

template <StorageFormat F>
void StaticGroup<F>::Clear()
{
  glDeleteBuffers(1, &mVbo);
  glDeleteBuffers(1, &mEab);
  glDeleteVertexArrays(1, &mVao);
}


}  // namespace gloo.