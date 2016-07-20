#include "camera.h"

namespace gloo
{

void Camera::OnWindowResize(int xo, int yo, int w, int h)
{
  mProjParameters.mAspect = static_cast<float>(w - xo) / static_cast<float>(h - yo);
  mProj.LoadIdentity();

  const float & mFovy = mProjParameters.mFovy;
  const float & mFarZ  = mProjParameters.mFarZ;
  const float & mNearZ = mProjParameters.mNearZ;
  const float & mAspect = mProjParameters.mAspect;

  mProj.Perspective(mFovy, mAspect, mNearZ, mFarZ);
}

void Camera::OnRendering()
{
  mView.LoadIdentity();

  // Set transform according to position, orientation and scales.
  // Since we are setting the camera, we need to do the inverse 
  // transform.
  mView.Translate(-mPos);
  mView.Rotate(-mRot[2], 0, 0, 1);
  mView.Rotate(-mRot[0], 1, 0, 0);
  mView.Rotate(-mRot[1], 0, 1, 0);
  mView.Scale(mScale);
}

void Camera::SetUniformModelViewProj(unsigned uniformLoc)
{
  // Compute MVP = P * V * M, where M is identity here.
  mProj.PushMatrix();  // Save P.
  mProj.Combine(mView);          // P = P * V.
  mProj.SetUniform(uniformLoc);  // Upload P.
  mProj.PopMatrix();   // Restore P.
}

void Camera::SetUniformModelViewProj(unsigned uniformLoc, const Transform & model)
{
  // Compute MVP = P * V * M.
  mProj.PushMatrix();  // Save P.
  mProj.Combine(mView);          // P = P * V.
  mProj.Combine(model);          // P = P * M.
  mProj.SetUniform(uniformLoc);  // Upload P.
  mProj.PopMatrix();   // Restore P.
}

glm::vec3 Camera::ComputeRayAt(float x_v, float y_v, float w, float h) const
{
  // Compute boundaries of near clip plane.
  float x_max = 1.0f;
  float y_max = 1.0f;

  // Convert viewport coordinates to projection plane coordinates.
  y_v = (h - y_v);
  float xp = ((x_v / w) * (2*x_max)) - x_max;
  float yp = ((y_v / h) * (2*y_max)) - y_max; 

  const glm::mat4& V = mView.GetMatrix();
  const glm::mat4& P = mProj.GetMatrix();

  glm::vec4 ray = glm::inverse(P*V) * glm::vec4(xp, yp, 1.0, 1.0);

  return glm::vec3(ray[0], ray[1], ray[2]);
}

}  // namespace gloo.