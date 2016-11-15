// + ======================================== +
// |         gl-oo-interface library          |
// |            Module: GLOO Debug.           |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +

#include "gloo/gl_header.h"

#include "gloo/group.h"
#include "transform.h"

// ============================================================================================= //
// This file provides a set of useful meshes for debugging, such as
// xyz-axis, grids, bounding boxes, bounding spheres and so on.
// They require a very simple shader that considers positions and 
// colors only. You can find this shaders in the folder '../../shaders'.
//
// The use is very straightforward:
//   1. Create an instance:
//     BoundingBoxMesh* mesh = new BoundingBoxMesh(renderer->GetPositionLoc(), 
//                                                 renderer->GetColorLoc(), 1, 0, 0);
//   2. Render whenever you want:
//     mesh->Render();
// 
//   3. Update according to its specific Update() method:
//     mesh->Update(-2, +2, 0, 1, 0, 1);  // BB.
//
//   4. Delete after its use:
//     delete mesh;
//
// ============================================================================================= //

#pragma once

namespace gloo
{

struct AxisMesh
{
public:
  AxisMesh(GLint positionAttribLoc, GLint colorAttribLoc);
  ~AxisMesh();

  void Render() const;
  void Update(GLfloat x, GLfloat y, GLfloat z);

  const MeshGroup<Batch>* GetMeshGroup() const { return mMeshGroup; }

private:
  MeshGroup<Batch>* mMeshGroup;
};

struct BoundingBoxMesh
{
public:
  BoundingBoxMesh(GLint positionAttribLoc, GLint colorAttribLoc,
                  GLfloat r = 0.8f, GLfloat g = 0.8f, GLfloat b = 0.8f);
  ~BoundingBoxMesh();

  void Render() const;
  void Update(GLfloat xmin, GLfloat xmax, GLfloat ymin, GLfloat ymax, GLfloat zmin, GLfloat zmax);

  const MeshGroup<Batch>* GetMeshGroup() const { return mMeshGroup; }

private:
  MeshGroup<Batch>* mMeshGroup;
};

struct GridMesh
{
  public:
  GridMesh(GLint positionAttribLoc, GLint colorAttribLoc, int width, int height, GLfloat tileSize = 1.0f,
           GLfloat r = 0.8f, GLfloat g = 0.8f, GLfloat b = 0.8f);
  ~GridMesh();

  void Render() const;

  const MeshGroup<Interleave>* GetMeshGroup() const { return mMeshGroup; }

private:
  MeshGroup<Interleave>* mMeshGroup;
};

struct Polygon
{
public:
  Polygon(GLint positionAttribLoc, GLint colorAttribLoc, float sideLength, int numSides = 4,
          float r = 0.8f, float g = 0.8f, float b = 0.8f, float xc = 0.0f, float yc = 0.0f);
  ~Polygon();

  void Render() const;
  void Update();

  const MeshGroup<Batch>* GetMeshGroup() const { return mMeshGroup; }

private:
  MeshGroup<Batch>* mMeshGroup;
};


}  // namespace gloo.