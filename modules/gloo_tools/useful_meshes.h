// + ======================================== +
// |         gl-oo-interface library          |
// |            Module: GLOO Debug.           |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +

#include "gl_header.h"

#include "gloo/group.h"
#include "transform.h"

// ============================================================================================= //
// This file provides a set of useful meshes for debugging, such as
// xyz-axis, grids, bounding boxes, bounding spheres and so on.
// They require a very simple shader that considers positions and 
// colors only. You can find this shaders in the folder '../../shaders'.
// ============================================================================================= //

namespace gloo
{


struct AxisMesh
{
public:
  AxisMesh(GLint positionAttribLoc, GLint colorAttribLoc);
  ~AxisMesh();

  void Render() const;
  void Update(GLfloat x, GLfloat y, GLfloat z);

private:
  MeshGroup<Batch>* mMeshGroup;
};

struct BoundingBoxMesh
{
public:
  BoundingBoxMesh(GLint positionAttribLoc, GLint colorAttribLoc);
  ~BoundingBoxMesh();

  void Render() const;
  void Update(GLfloat xmin, GLfloat xmax, GLfloat ymin, GLfloat ymax, GLfloat zmin, GLfloat zmax);

private:
  MeshGroup<Batch>* mMeshGroup;
};


}  // namespace gloo.