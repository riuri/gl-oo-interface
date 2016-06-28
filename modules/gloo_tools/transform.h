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
//  It stores a matrix and a stack of matrices so that
//  any hierarchical transform can be modeled. All me-
//  thods modify the current matrix, which carries all
//  the transform so far.
//  Whenever a method such as Rotate() or Perspective()
//  is called, the current matrix is changed to accumulate
//  the transform (by multiplying by the right side).
//
//  ----------------------------------------------------------------------
//  INSTRUCTIONS
//
//  1. Simple example (as a model matrix):
//  gloo::Transform model();
//  model.Translate(0, 0, -1.0f);
//  model.Rotate(M_PI/2.0f, 0, 0, 1);
//  model.SetUniform(shaderProgram->GetHandle(), "modelMatrix");
//
//  2. In order to use in a hierarchical rendering, the methods
//  PushMatrix and PopMatrix must be called, so that the curent
//  transform is saved in the matrices stack. 
//
//  3. GetMatrix() returns the current transform, either as glm::mat4
//  or a double column-major array. GetInverseMatrix() does the same
//  routine, but using the inverse of the current matrix.
//
//  4. The operator << to ostream is overloaded. Then, you're allowed
//  to do:   std::cout << model << std::endl;
//  It will print the current matrix. For more detailed information 
//  about the entire stack, use PrintStack().
//
//  5. The default assignment operator works. So, you're allowed to use
//  gloo::Transform copy = model;
//
//  6. You can combine transforms using Combine():
//  MVP.Perspective(...);     // Set projection 
//  MVP.Combine(modelView);   // Combine with modelView (right side)
//  MVP.SetUniform(shaderProgram->GetHandle(), "MVP");
//  ----------------------------------------------------------------------

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>

namespace gloo
{

class Transform
{
public:
  // -> Constructor/Destructor.
  // Constructs Transform and loads identity in it.
  Transform();
  
  ~Transform() 
  { }

  // Uploads current matrix to GPU (in an uniform in shader program).
  // Note: the corresponding shader has to be binded so that the name
  // is successfully retrieved.
  void SetUniform(unsigned programHandle, const std::string & uniformName) const;
  void SetUniform(unsigned uniformHandler) const;

  // Has the same functionality as SetUniform, but using the inverse current matrix.
  void SetInverseUniform(unsigned programHandle, const std::string & uniformName) const;
  void SetInverseUniform(unsigned uniformHandler) const;

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
  void LookAt(const glm::vec3 & eye, 
              const glm::vec3 & center, 
              const glm::vec3 & up);

  // Inverts the current matrix.
  void Invert();

  // Transposes the current matrix (good for rotations' inverse).
  void Transpose();

  // Multiply current matrix by m.
  void MultMatrix(const glm::mat4 & m);
  void MultMatrix(const float* m);  // Column-major.

  // -> Methods for setting the current matrix as projective transformation.
  // Specify the projection type and the corresponding parameters (limits or aspect).
  void Ortho(      float left, float right, float bottom, float top, float zNear, float zFar);
  void Frustum(    float left, float right, float bottom, float top, float zNear, float zFar);
  void Perspective(float fovY, float aspect, float zNear, float zFar);

  // -> Hierachy/Chain manipulation methods.
  void PushMatrix();
  void PopMatrix();

  void PushAndLoadIdentity();

  // -> Load/Query methods.
  glm::mat4 GetMatrix() const;     // Returns 4x4 matrix which represents the entire transformation (current).
  void GetMatrix(float* m) const;  // Stores  4x4 matrix which represents the entire transformation into m.

  glm::mat4 GetInverseMatrix() const;     // Provides the inverse matrix of GetMatrix().
  void GetInverseMatrix(float* m) const;  // Provides the inverse matrix of GetMatrix().

  void LoadIdentity();                   // Sets the current matrix to be identity 4x4.
  void LoadMatrix(const glm::mat4 & m);  // Sets the current matrix to be m.
  void LoadMatrix(const float* m);       // Sets the current matrix to be m (column-major).

  // -> Utilities/Log.

  // Combines current transform with a second transform.
  // The resulting combination is stored in the first transform.
  void Combine(const Transform & other);

  // Prints current matrix.
  friend std::ostream& operator<<(std::ostream& os, const Transform& transform);

  // Prints the entire matrix stack. Precision, width and fixed specifies formatting properties.
  void PrintStack(int precision = 6, int width = 12, bool fixed = true) const;

  // Matrix to std::string.
  static std::string MatrixToStr(const glm::mat4 & m, int precision = 6, 
                                 int width = 12, bool fixed = true);

private:
  std::vector<glm::mat4> mStack;  // Stack of transforms.
  glm::mat4 mCurrent;             // Current matrix.
};


}  // namespace gloo.