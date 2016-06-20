// + ======================================== +
// |         gl-oo-interface library          |
// |            Module: GLOO Tool.            |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +
//
//  gloo::Transform implements geometry transform
//  of an object/mesh or anything by combining
//  individual rotations/translations and scales.
//  It also implements projective transforms.
//
//  The base for the class is the OpenGL Mathematics
//  library: http://glm.g-truc.net/0.9.7/index.html
//
//  It is based on the starter code provided by
//  Professor Jernej Barbic and his TA Bohan Wang
//  for CSCI 420 at USC. 
//

#pragma once

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

namespace gloo
{

class Transform
{
public:
  Transform();
  ~Transform();

  // -> Methods for setting the current matrix as model/view transformation.
  // Rotates theta degrees around axis [x, y, z]'.
  void Rotate(float theta, float x, float y, float z);
  void Rotate(float theta, const glm::vec3 & axis);

  // Scales with factors x, y, z for the corresponding axis.
  void Scale(float x, float y, float z);
  void Scale(const glm::vec3 & scale);
    
  // Translates of [x, y, z]'.
  void Translate(float x, float y, float z);
  void Translate(const glm::vec3 & translation);

  // Sets the camera position, aim and orientation.
  void LookAt(float eyeX,    float eyeY,    float eyeZ, 
              float centerX, float centerY, float centerZ, 
              float upX,     float upY,     float upZ);

  // Inverts the current matrix.
  void Invert();  // ------------------- OK

  // Transposes the current matrix (good for rotations' inverse).
  void Transpose();  // ------------------- OK

  // Multiply current matrix by m.
  void MultMatrix(const glm::mat4 & m);  // ------------------- OK
  void MultMatrix(const float* m);  // Column-major.

  // -> Methods for setting the current matrix as projective transformation.
  // Specify the projection type and the corresponding parameters (limits or aspect).
  void Ortho(float left, float right, float bottom, float top, float zNear, float zFar);
  void Frustum(float left, float right, float bottom, float top, float zNear, float zFar);
  void Perspective(float fovY, float aspect, float zNear, float zFar);

  // -> Hierachy/Chain manipulation methods.
  void PushMatrix();
  void PopMatrix();

  void PushAndLoadIdentity();
  void PopAndLoadIdentity();

  // -> Load/Query methods.
  glm::mat4 GetTransform();     // Returns the 4x4 matrix which represents the entire transformation.
  void GetTransform(float* m);  // Stores the 4x4 matrix which represents the entire transformation into m.

  glm::mat4 GetInverseTransform();     // Provides the inverse matrix of GetTransform().
  void GetInverseTransform(float* m);  // Provides the inverse matrix of GetTransform().

  void LoadIdentity();                      // Sets the current matrix to be identity 4x4.
  void LoadMatrix(const glm::mat4 & m);  // Sets the current matrix to be m.
  void LoadMatrix(const float* m);       // Sets the current matrix to be m (column-major).

  glm::mat4 GetMatrix();     // Returns the current glm matrix.
  void GetMatrix(float* m);  // Stores the current matrix into m (column-major).

private:
  std::vector<glm::mat4> mStack;  // Stack of transforms.
  glm::mat4 mCurrent;             // Current matrix.
};


}  // namespace gloo