// + ======================================== +
// |         gl-oo-interface library          |
// |            Module: GLOO Mesh.            |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +

// ============================================================================================= //
// Group
// ============================================================================================= //
// 3D-surface meshes are made up by groups, which are parts that share the same
// materials or textures.
// Group has the following features:

// [StorageFormat]
//
// Each group has a single vertex buffer in GPU, which can follow two kinds of storage:
// 1. Interleaved (Tighly Packed):  (P N T) (P N T) ... (P N T)
// 2. Batched (Sub-Buffered):       (P P ... P) (N N ... N) (T T ... T)
// Where P is the vertex positions array, N is the vertex normals array and so on.
//
// The storage format is provided by the template parameter <StorageFormat F>. 

// [Vertex Attribute Data]
//
// A vertex attribute list must be specified to a group so all data can be successfully passed
// to the shader during rendering or updating methods.
// Vertex attribute list (or data) basically defines which properties a vertex contain.
//
// For example, the list {{3, position_loc}, {3, color_loc}} defines two attributes:
// 1. {3, position_loc} : dimensionality 3 (3 floating points) and shader location position_loc;
// 2. {3, color_loc}    : dimensionality 3 (3 floating points) and shader location color_loc;
// The location in shaders must be coeherent to attributes you've written on your shader.
//
// Vertex attribute data is specified through method SetVertexAttribList().

// [Loading/updating data]
// 
// In order to load data to GPU buffers, you can either pass as parameter the raw buffer
// containing all arranged data or pass a list of separate buffers containing per attribute
// data.
// You can do it using the two overloaded versions of Load().
// If you don't to provide elements, just pass nullptr to 'indices' parameter and Load() will
// automatically build the element array for you (following the default order).

// [USAGE]
/*
    // Create.
    MeshGroup<Batch>* group = new MeshGroup<Batch>(numVertices, numElements, GL_TRIANGLES);
 
    // Specify vertex attribute data.
    group->SetVertexAttribList({{3, position_loc}, {3, normal_loc}, {2, uv_loc}});

    // Load data into GPU (from separate buffers).
    group->Load({positions.data(), normals.data(), uv.data()}, indices.data());
    ...
    // Render!
    group->Render();
    ...
    // Don't forget to delete it.
    delete group;
*/

// IMPORTANT:
// If you want to use a group in different shader programs, you should 
// make sure that all of them have the same attribute location (typically,
// loc=0 for position, loc=1 for normal and so on).
//
// ============================================================================================= //

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
  MeshGroup(int numVertices, int numElements, GLenum drawMode = GL_TRIANGLE_STRIP,
            GLenum dataUsage = GL_STATIC_DRAW);

  ~MeshGroup();

  // Specifies how data in stored per vertex.
  void SetVertexAttribList(std::initializer_list<VertexAttribute> vertexAttribList);

  // Should be called on display function (it calls glDrawElements).
  void Render() const;

  // TODO: document.
  bool Load(const std::vector<GLfloat*> & bufferList, const GLuint* indices);
  bool Load(const GLfloat* buffer, const GLuint* indices);

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
  GLenum mDrawMode;     // How mesh is rendered (drawing mode).
  GLenum mDataUsage;    // Tells if buffers are dynamic/static or for reading/writing.

  GLuint mVertexSize;   // Number of floating points stored per vertex.
  GLuint mNumVertices;  // Number of vertices in this group.
  GLuint mNumElements;  // Number of elements (indices of vertex).

  // Vertex attributes descriptor -> specifies which attributes a vertex contain and also
  // their dimensionality and order. This is constant within the lifetime of a MeshGroup.
  std::vector<VertexAttribute> mVertexAttributeList;
};

// ============================================================================================ //
// Implementation of template functions.

/* Constructor */
template <StorageFormat F>
MeshGroup<F>::MeshGroup(int numVertices, int numElements, GLenum drawMode, GLenum dataUsage)
: mNumVertices(numVertices)
, mNumElements(numElements)
, mDataUsage(dataUsage)
, mDrawMode(drawMode)
{

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

template <StorageFormat F>
void MeshGroup<F>::SetVertexAttribList(std::initializer_list<VertexAttribute> vertexAttribList)
{
  mVertexAttributeList = std::vector<VertexAttribute>(vertexAttribList);

  mVertexSize = 0;
  for (auto & attribute : mVertexAttributeList) 
  {
    mVertexSize += attribute.mSize;
  }
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
               vertices, mDataUsage);

  MeshGroup<F>::BuildVAO();
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
bool MeshGroup<Batch>::Load(
    const std::vector<GLfloat*> & bufferList, const GLuint* indices);

template <>
bool MeshGroup<Batch>::Load(
    const GLfloat* buffer, const GLuint* indices);

template <>
void MeshGroup<Batch>::BuildVAO();

// ================ Interleaved Storage ==================== // 

template <>
bool MeshGroup<Interleave>::Load(
    const std::vector<GLfloat*> & bufferList, const GLuint* indices);

template <>
bool MeshGroup<Interleave>::Load(
    const GLfloat* buffer, const GLuint* indices);

template <>
void MeshGroup<Interleave>::BuildVAO();




}  // namespace gloo.