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
// 3. On rendering function call OnRendering() before objects.
// 4. 
//
//
//
// NOTE: if you are using a Renderer class, just set the camera
// to it and all the main work will be done by the renderer.

#pragma once

#include <glm/glm.hpp>

#include "gl_header.h"
#include "transform.h"

namespace gloo
{

struct ProjectionParameters
{
  float mFovy   { M_PI/3.0 } ;  // Field of view of Y [angle in radians].
  float mFarZ   {  0.1  };      // Maximum rendering distance.
  float mNearZ  { 100.0 };      // Minimum rendering distance.
  float mAspect { 4.0/3.0 } ;   // Ratio a = W / H  [width/height].
};

class Camera
{
public:
  // Constructors.
  Camera() { }
  Camera(const ProjectionParameters & projParameters)
  : mProjParameters(projParameters) { }
  Camera(ProjectionParameters && projParameters)
  : mProjParameters(projParameters) { }

  ~Camera();

  // 1. Main methods -> must be called when rendering.

  // Must be called when the viewport changes (changes internal projection parameters).
  void OnWindowResize(int xo, int yo, int w, int h);

  // Must be called before rendering objects (if you want to use this camera as the active one).
  void OnRendering();

  // 2. Setter methods -> change the internal parameters.

  // Set camera propeties -> glm::vec3.
  void SetPosition(const glm::vec3 & pos) { mPos = pos; }       // pos = [xc, yc, zc]'
  void SetRotation(const glm::vec3 & rot) { mRot = rot; }       // rot = [rx, ry, rz]'
  void SetScale(const glm::vec3 & scales) { mScale = scales; }  // scales = [sx, sy, sz]'

  // Set camera propeties -> 3 floats.
  void SetPosition(float xc, float yc, float zc);
  void SetRotation(float rx, float ry, float rz);
  void SetScale(float sx, float sy, float sz);

  // 3. Getter methods.
  glm::vec3 GetPosition() const { return mPos; }
  glm::vec3 GetRotation() const { return mRot; }
  glm::vec3 GetScale() const  { return mScale; }
  // TODO: support different ways of getting vectors.

  const Transform & ViewTransform() const { return mView; };
  const Transform & ProjTransform() const { return mProj; };

  Transform & ViewTransform() { return mView; };
  Transform & ProjTransform() { return mProj; };

protected:
  glm::vec3 mPos   { 0, 0, 1 };  // Center coordinates.
  glm::vec3 mRot   { 0, 0, 0 };  // Rotation angles in x, y, z axis (orientation).
  glm::vec3 mScale { 1, 1, 1 };  // Scaling factors for x, y, z axis.

  // Transform stacks.
  Transform mView;  // Specifies camera position, orientation and so on [a stack].
  Transform mProj;  // Specifies projective transform.
  ProjectionParameters mProjParameters;  // Specifies the type of projection.
};

// =========== IMPLEMENTATION OF INLINE METHODS ===============

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

}  // namespace gloo.