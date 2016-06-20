#include "transform.h"

#include <vector>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gloo 
{

Transform::Transform()
{
  // Initialize first matrix with identity.
  mCurrent = glm::mat4(1.0f);
}

Transform::~Transform()
{

}

// void Transform::Rotate(float angle, float x, float y, float z)
// {
//   glm::mat4 R = glm::rotate(angle, glm::vec3(x, y, z));
//   multiplyMatrixToCurrent(R);
// }

// void Transform::Translate(float x, float y, float z)
// {
//   glm::mat4 T = glm::translate(glm::vec3(x, y, z));
//   multiplyMatrixToCurrent(T);
// }

// void Transform::Scale(float x, float y, float z)
// {
//   glm::mat4 S = glm::scale(glm::vec3(x, y, z));
//   multiplyMatrixToCurrent(S);
// }

// void Transform::LookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
// {
//   glm::mat4 V = glm::lookAt(glm::vec3(eyeX, eyeY, eyeZ), 
//                             glm::vec3(centerX, centerY, centerZ), 
//                             glm::vec3(upX, upY, upZ));
//   multiplyMatrixToCurrent(V);
// }

// void Transform::Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
// {
//   glm::mat4 O = glm::ortho(left, right, bottom, top, zNear, zFar);
//   multiplyMatrixToCurrent(O);
// }

// void Transform::Frustum(float left, float right, float bottom, float top, float zNear, float zFar)
// {
//   glm::mat4 F = glm::frustum(left, right, bottom, top, zNear, zFar);
//   multiplyMatrixToCurrent(F);
// }

// void Transform::Perspective(float fovY, float aspect, float zNear, float zFar)
// {
//   glm::mat4 P = glm::perspective(fovY, aspect, zNear, zFar);
//   multiplyMatrixToCurrent(P);
// }

// void Transform::SetMatrixMode(MatrixMode matrixMode) 
// { 
//   this->matrixMode = matrixMode; 
// }

// void Transform::LoadIdentity()
// {
//   currentMatrix[(int)matrixMode] = glm::mat4(1.0f);
// }

// void Transform::LoadMatrix(const float * matrix)
// {
//   currentMatrix[(int)matrixMode] = glm::make_mat4(matrix);
// }

// void Transform::MultMatrix(const float * matrix)
// {
//   glm::mat4 M = glm::make_mat4(matrix);
//   multiplyMatrixToCurrent(M);
// }

// void Transform::PushMatrix()
// {
//   matrixStack[(int)matrixMode].push_back(currentMatrix[(int)matrixMode]);
// }

// void Transform::PopMatrix()
// {
//   if (matrixStack[(int)matrixMode].size() >= 1) // cannot pop from an empty stack
//   {
//     currentMatrix[(int)matrixMode] = matrixStack[(int)matrixMode].back();
//     matrixStack[(int)matrixMode].pop_back();
//   }
// }

// void Transform::GetMatrix(float * matrix)
// {
//   memcpy(matrix, glm::value_ptr(currentMatrix[(int)matrixMode]), sizeof(float) * 16);
// }

// void Transform::GetNormalMatrix(float * matrix)
// {
//   // glm::mat4 MV(currentMatrix[(int)matrixMode]);
//   // glm::mat4 NMV = glm::transpose(glm::inverse(MV));
//   // memcpy(matrix, glm::value_ptr(NMV), sizeof(float) * 16);
// }

void Transform::Invert()
{
  mCurrent = glm::inverse(mCurrent);
}

void Transform::Transpose()
{
  mCurrent = glm::transpose(mCurrent);
}

void Transform::MultMatrix(const glm::mat4 & m)
{
  mCurrent = mCurrent * m;
}

// void Transform::GetProjectionModelViewMatrix(float * matrix)
// {
//   glm::mat4 PM = currentMatrix[Projection] * currentMatrix[ModelView];
//   memcpy(matrix, glm::value_ptr(PM), sizeof(float) * 16);
// }

}  // namespace gloo.