// + ======================================== +
// |         gl-oo-interface library          |
// |            Module: GLOO Tool.            |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +
//
// gloo::Camera implements a general purpose camera
// for any 3d application with perspective projecton.
// 
//
//
// INSTRUCTIONS
//
// 1. Create a camera:  gloo::Camera* camera = new Camera( );
//     [Optionally pass ProjectiveParameters as arguments ^ ]
// 2. On window resize/reshape, call OnWindowResize().
// 3. On rendering function call SetOnRendering() before rendering objects.
// 
// ...
//
// You can also manually set the internal transforms by calling
// ViewTransform() and ProjTransform() getter methods.
// For more info, see "gloo_tools/transform.h"
//
// NOTE: if you are using a Renderer class, just set the camera
// to it and all the main work will be done by the renderer.
//
// NOTE2: by calling getter methods for Transforms, you'll get
// the transform set from the last frame.

#pragma once

#include <glm/glm.hpp>

#include "gloo/gl_header.h"
#include "transform.h"

namespace gloo
{

// This structure stores the perspective projection attributes
// of this camera.
struct ProjectionParameters
{
  float mFovy   { M_PI/3.0 } ;  // Field of view in Y [angle in radians].
  float mFarZ   { 100.0 };      // Maximum rendering distance.
  float mNearZ  {   0.1 };      // Minimum rendering distance.
  float mAspect { 4.0/3.0 } ;   // Ratio a = W / H  [width/height].
};

class Camera
{
public:
  // Constructors.
  Camera() { }
  Camera(ProjectionParameters && projParameters)
  : mProjParameters(projParameters) { }
  Camera(const ProjectionParameters & projParameters)
  : mProjParameters(projParameters) { }

  ~Camera() { }

  // 1. Main methods -> must be called when rendering.
  //                 -> should be used to move/animate camera.
  //                 -> they provide a pretty interface to set uniform matrices.

  // Must be called when the viewport changes (changes internal projection parameters).
  void SetOnReshape(int xo, int yo, int w, int h);

  // Must be called before rendering objects (if you want to use this camera as the active one).
  // This method updates the internal matrices so they can be set as uniforms in the shader.
  void SetOnRendering();

  // The following methods set internal transforms as uniforms in shader.
  // Please call SetOnRendering() before setting uniforms.
  void SetUniformViewMatrix(unsigned uniformLoc);  // Just view matrix.
  void SetUniformProjMatrix(unsigned uniformLoc);  // Just projection matrix.
  void SetUniformViewProj(unsigned uniformLoc);    // Proj * View.
  void SetUniformModelView(unsigned uniformLoc, const Transform & model);      // View * Model.
  void SetUniformModelViewProj(unsigned uniformLoc, const Transform & model);  // Proj * View * Model.


  // Animate camera center, orientation and scale -> glm::vec3.
  void Translate(const glm::vec3 & dPos);
  void Rotate(const glm::vec3 & dRot);
  void Scale(const glm::vec3 & dScale);

  // Animate camera center, orientation and scale -> 3 floats.
  void Translate(float dx, float dy, float dz);
  void Rotate(   float dx, float dy, float dz);
  void Scale(    float dx, float dy, float dz);

  // 2. Setter methods -> change the internal parameters.

  // Set camera propeties -> glm::vec3.
  inline void SetPosition(const glm::vec3 & pos) { mPos = pos; }       // pos = [xc, yc, zc]'
  inline void SetRotation(const glm::vec3 & rot) { mRot = rot; }       // rot = [rx, ry, rz]'
  inline void SetScale(const glm::vec3 & scales) { mScale = scales; }  // scales = [sx, sy, sz]'

  // Set camera propeties -> 3 floats.
  void SetPosition(float xc, float yc, float zc);
  void SetRotation(float rx, float ry, float rz);
  void SetScale(float sx, float sy, float sz);

  // Set projection parameters.
  void SetProjectionParameters(const ProjectionParameters & projParameters);

  // 3. Getter methods.
  inline glm::vec3 GetPosition() const { return mPos; }
  inline glm::vec3 GetRotation() const { return mRot; }
  inline glm::vec3 GetScale() const  { return mScale; }

  inline const Transform & ViewTransform() const { return mView; };
  inline const Transform & ProjTransform() const { return mProj; };

  inline Transform & ViewTransform() { return mView; };
  inline Transform & ProjTransform() { return mProj; };

  const ProjectionParameters & GetProjectionParameters() const { return mProjParameters; }
  ProjectionParameters & GetProjectionParameters() { return mProjParameters; }

  // 4. Methods for selection/intersection.

  // Computes the vector which goes from camera center to mouse coordinates on projection plane.
  glm::vec3 ComputeRayAt(float x_v, float y_v, float w, float h) const;

protected:
  glm::vec3 mPos   { 0, 0, 1 };  // Center coordinates.
  glm::vec3 mRot   { 0, 0, 0 };  // Rotation angles in x, y, z axis (orientation).
  glm::vec3 mScale { 1, 1, 1 };  // Scaling factors for x, y, z axis.

  // Transform stacks.
  Transform mView;  // Specifies camera position, orientation and so on [a stack].
  Transform mProj;  // Specifies projective transform.
  ProjectionParameters mProjParameters;  // Specifies parameters of projection.
};

// =========== IMPLEMENTATION OF INLINE METHODS ===================================================

inline
void Camera::Translate(const glm::vec3 & dPos)
{
  mPos += dPos;
}

inline
void Camera::Rotate(const glm::vec3 & dRot)
{
  mRot += dRot;
}

inline
void Camera::Scale(const glm::vec3 & dScale)
{
  mScale[0] *= (1.0f + dScale[0]);
  mScale[1] *= (1.0f + dScale[1]);
  mScale[2] *= (1.0f + dScale[2]);
}

/// ===============================================================================================

inline
void Camera::Translate(float dx, float dy, float dz)
{
  mPos[0] += dx;
  mPos[1] += dy;
  mPos[2] += dz;
}

inline
void Camera::Rotate(float dx, float dy, float dz)
{
  mRot[0] += dx;
  mRot[1] += dy;
  mRot[2] += dz;
}

inline
void Camera::Scale(float dx, float dy, float dz)
{
  mScale[0] *= (1.0f + dx);
  mScale[1] *= (1.0f + dy);
  mScale[2] *= (1.0f + dz);
}

/// ===============================================================================================

inline
void Camera::SetPosition(float xc, float yc, float zc)
{
  mPos[0] = xc;
  mPos[1] = yc;
  mPos[2] = zc;
}

inline
void Camera::SetRotation(float rx, float ry, float rz)
{
  mRot[0] = rx;
  mRot[1] = ry;
  mRot[2] = rz;
}

inline
void Camera::SetScale(float sx, float sy, float sz)
{
  mScale[0] = sx;
  mScale[1] = sy;
  mScale[2] = sz;
}

/// ===============================================================================================

inline
void Camera::SetProjectionParameters(const ProjectionParameters & projParameters)
{
  mProjParameters = projParameters;
}

inline
void Camera::SetUniformViewMatrix(unsigned uniformLoc)
{
  mView.SetUniform(uniformLoc);
}

inline
void Camera::SetUniformProjMatrix(unsigned uniformLoc)
{
  mProj.SetUniform(uniformLoc);
}

}  // namespace gloo.