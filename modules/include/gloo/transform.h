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
#include <glm/glm.hpp>

namespace gloo
{

class Transform
{
public:
  Transform();
  ~Transform();

  // -> Methods for setting the top matrix as model/view transformation.
  // Rotates theta degrees around axis [x, y, z]'.
  void Rotate(float theta, float x, float y, float z);
  void Rotate(float theta, const glm::vec3 & axis);

  // Scales with factors x, y, z for the corresponding axis.
  void Scale(float x, float y, float x);
  void Scale(const glm::vec3 & scale);
    
  // Translates of [x, y, z]'.
  void Translate(float x, float y, float z);
  void Translate(const glm::vec3 & translation);
  
  // -> Methods for setting the top matrix as projective transformation.
  // Specify the projection type and the corresponding parameters (limits or aspect).
  void Ortho(float left, float right, float bottom, float top, float zNear, float zFar);
  void Frustum(float left, float right, float bottom, float top, float zNear, float zFar);
  void Perspective(float fovY, float aspect, float zNear, float zFar);

  // -> Hierachy/Chain manipulation methods.



  // -> Getter/Setter methods.
  glm::mat4 GetTopMatrix();     // Returns the top glm matrix.
  void GetTopMatrix(float* m);  // Stores the top matrix into m (column-major).

  void SetTopMatrix(const glm::mat4 & m);  // Sets the top matrix to be m.
  void SetTopMatrix(const float* m);       // Sets the top matrix to be m (column-major).

  

private:
  // Stack of transforms.
  std::vector<glm::mat4> mStack;  
};


}  // namespace gloo