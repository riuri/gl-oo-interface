#include "transform.h"

#include <iomanip>
#include <sstream>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gloo 
{

// -- 1
Transform::Transform()
{
  // Initialize first matrix with identity.
  mCurrent = glm::mat4(1.0f);
}

// ------------------------------------------------------------------------------------------------
// -> Methods for setting the current matrix as model/view transformation.

// -- 2
void Transform::Rotate(float theta, float x, float y, float z)
{
  Transform::MultMatrix( glm::rotate(theta, glm::vec3(x, y, z)) );
}

void Transform::Rotate(float theta, const glm::vec3 & axis)
{
  Transform::MultMatrix( glm::rotate(theta, axis) );
}

// -- 3
void Transform::Scale(float x, float y, float z)
{
  Transform::MultMatrix( glm::scale(glm::vec3(x, y, z)) );
}

void Transform::Scale(const glm::vec3 & scale)
{
  Transform::MultMatrix( glm::scale(scale) );
}

// -- 4
void Transform::Translate(float x, float y, float z)
{
  Transform::MultMatrix(  glm::translate(glm::vec3(x, y, z)) );
}

void Transform::Translate(const glm::vec3 & translation)
{
  Transform::MultMatrix(  glm::translate(translation) );
}

// -- 5
void Transform::LookAt(float eyeX,    float eyeY,    float eyeZ, 
                       float centerX, float centerY, float centerZ, 
                       float upX,     float upY,     float upZ)
{
  glm::mat4 V = glm::lookAt(glm::vec3(eyeX, eyeY, eyeZ), 
                            glm::vec3(centerX, centerY, centerZ), 
                            glm::vec3(upX, upY, upZ));

  Transform::MultMatrix(V);
}

void Transform::LookAt(const glm::vec3 & eye, 
                       const glm::vec3 & center, 
                       const glm::vec3 & up)
{
  Transform::MultMatrix( glm::lookAt(eye, center, up) );
}

// -- 6
void Transform::Invert()
{
  mCurrent = glm::inverse(mCurrent);
}

void Transform::Transpose()
{
  mCurrent = glm::transpose(mCurrent);
}

// -- 7
void Transform::MultMatrix(const glm::mat4 & m)
{
  mCurrent = mCurrent * m;
}

void Transform::MultMatrix(const float* m)
{
  Transform::MultMatrix(glm::make_mat4(m));
}

// ------------------------------------------------------------------------------------------------
// -> Methods for setting the current matrix as projective transformation.

// -- 8
void Transform::Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
  Transform::MultMatrix( glm::ortho(left, right, bottom, top, zNear, zFar) );
}

void Transform::Frustum(float left, float right, float bottom, float top, float zNear, float zFar)
{
  Transform::MultMatrix( glm::frustum(left, right, bottom, top, zNear, zFar) );
}

void Transform::Perspective(float fovY, float aspect, float zNear, float zFar)
{
  Transform::MultMatrix( glm::perspective(fovY, aspect, zNear, zFar) );
}

// ------------------------------------------------------------------------------------------------
// -> Hierachy/Chain manipulation methods.

// -- 9
void Transform::PushMatrix()
{
  mStack.push_back(mCurrent);
}

void Transform::PopMatrix()
{
  if (mStack.size() >= 1) 
  {
    mCurrent = mStack.back();
    mStack.pop_back();
  }
}

void Transform::PushAndLoadIdentity()
{
  Transform::PushMatrix();
  mCurrent = glm::mat4(1.0f);
}

// ------------------------------------------------------------------------------------------------
// -> Load/Query methods.

// -- 10
glm::mat4 Transform::GetMatrix()
{
  return mCurrent;
}

void Transform::GetMatrix(float* m)
{
  memcpy(m, glm::value_ptr(mCurrent), sizeof(float) * 16);
}

glm::mat4 Transform::GetInverseMatrix()
{
  return glm::inverse(mCurrent);
}

void Transform::GetInverseMatrix(float* m)
{
  glm::mat4 mCurrentInv = glm::inverse(mCurrent);
  memcpy(m, glm::value_ptr(mCurrentInv), sizeof(float) * 16);
}

void Transform::LoadIdentity()
{
  mCurrent = glm::mat4(1.0f);
}

void Transform::LoadMatrix(const glm::mat4 & m)
{
  mCurrent = m;
}

void Transform::LoadMatrix(const float* m)
{
  mCurrent = glm::make_mat4(m);
}

// ------------------------------------------------------------------------------------------------
// -> Utilities/Log.

// -- 11

std::ostream& operator<<(std::ostream& os, const Transform& transform)
{
  os << Transform::MatrixToStr(transform.mCurrent);
  return os;
}

void Transform::PrintStack(int precision, int width, bool fixed)
{
  std::cout << "Current matrix =\n" 
            << Transform::MatrixToStr(mCurrent, precision, width, fixed) << std::endl;
  
  std::cout << "-------------------------- Top  --------------------------" << std::endl;
  for (int i = mStack.size()-1; i >= 0; i--) 
  {
    std::cout << "S[" << i << "] = \n" 
              << Transform::MatrixToStr(mStack[i], precision, width, fixed) << std::endl;
  }
  std::cout << "------------------------ Bottom --------------------------" << std::endl << std::endl;
}

std::string Transform::MatrixToStr(const glm::mat4 & m, int precision, int width, bool fixed) 
{
  std::ostringstream oss;
  oss << std::setprecision(precision);
  oss << std::setw(width);
  
  if (fixed) 
  {
    oss << std::fixed;
  }

  const float * data = glm::value_ptr(m);
  for (int i = 0; i < 4; i++) 
  {
    for (int j = 0; j < 4; j++)
    {
      // Column-major.
      oss << std::setw(width) << data[4*j + i] << "  ";
    }
    oss << std::endl;
  }

  return oss.str();
}

}  // namespace gloo.












