#include "useful_meshes.h"

// #include ""

namespace gloo
{

// ============================================================================================= //

AxisMesh::AxisMesh(GLint positionAttribLoc, GLint colorAttribLoc)
{
  const int numVertices = 6;
  const int numElements = 6;
  const GLenum drawMode = GL_LINES;

  // Initialize vertices.
  GLfloat positions[] = {0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
                         0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f};
  GLfloat colors [] =   {1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
                         0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
                         0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f};
  const GLuint* indices = nullptr;

  // Allocate mesh.
  mMeshGroup = new MeshGroup<Batch>(numVertices, numElements, drawMode);

  // Specify its attributes.
  mMeshGroup->SetVertexAttribList({3, 3});

  // Add rendering pass.
  mMeshGroup->AddRenderingPass({{positionAttribLoc, true}, {colorAttribLoc, true}});

  // Load data.
  mMeshGroup->Load({positions, colors}, indices);
}

AxisMesh::~AxisMesh()
{
  delete mMeshGroup;
}

void AxisMesh::Update(GLfloat x, GLfloat y, GLfloat z)
{
  GLfloat positions[] = {0.0f, 0.0f, 0.0f,     x, 0.0f, 0.0f,
                         0.0f, 0.0f, 0.0f,  0.0f,    y, 0.0f,
                         0.0f, 0.0f, 0.0f,  0.0f, 0.0f,    z};

  mMeshGroup->Update({positions, nullptr});
}

void AxisMesh::Render() const
{
  mMeshGroup->Render();
}

// ============================================================================================= //

BoundingBoxMesh::BoundingBoxMesh(GLint positionAttribLoc, GLint colorAttribLoc)
{
  const int numVertices = 8;
  const int numElements = 12*2;
  const GLenum drawMode = GL_LINES;

  // Initialize vertices.
  GLfloat positions[] = {-0.5f, +0.5f, +0.5f,  +0.5f, +0.5f, +0.5f,
                         -0.5f, +0.5f, -0.5f,  +0.5f, +0.5f, -0.5f,
                         -0.5f, -0.5f, +0.5f,  +0.5f, -0.5f, +0.5f,
                         -0.5f, -0.5f, -0.5f,  +0.5f, -0.5f, -0.5f };

  GLfloat colors [] =   {0.7f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f,
                         0.7f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f,
                         0.7f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f,
                         0.7f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f};
  GLuint indices[] = { 0,1, 2,3, 4,5, 6,7,    // x
                       0,2, 1,3, 4,6, 5,7,    // z
                       0,4, 1,5, 2,6, 3,7 };  // z

  // Allocate mesh.
  mMeshGroup = new MeshGroup<Batch>(numVertices, numElements, drawMode);

  // Specify its attributes.
  mMeshGroup->SetVertexAttribList({3, 3});

  // Add rendering pass.
  mMeshGroup->AddRenderingPass({{positionAttribLoc, true}, {colorAttribLoc, true}});

  // Load data.
  mMeshGroup->Load({positions, colors}, indices);
}

BoundingBoxMesh::~BoundingBoxMesh()
{
  delete mMeshGroup;
}

void BoundingBoxMesh::Update(GLfloat xmin, GLfloat xmax, GLfloat ymin, GLfloat ymax, GLfloat zmin, GLfloat zmax)
{
  GLfloat positions[] = {xmin, ymax, zmax,  xmax, ymax, zmax,
                         xmin, ymax, zmin,  xmax, ymax, zmin,
                         xmin, ymin, zmax,  xmax, ymin, zmax,
                         xmin, ymin, zmin,  xmax, ymin, zmin };

  mMeshGroup->Update({positions, nullptr});
}

void BoundingBoxMesh::Render() const
{
  mMeshGroup->Render();
}

}  // namespace gloo.