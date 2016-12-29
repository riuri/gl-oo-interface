// + ======================================== +
// |         gl-oo-interface library          |
// |         Module: GLOO Rendering.          |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +

//  TODO(Castiel): write documentation.

#pragma once

#include <vector>
#include <iostream>
#include <initializer_list>

#define GLM_SWIZZLE
#include "gloo/gl_header.h"
#include "gloo/transform.h"

namespace gloo
{

const GLint kNoUniform = -1;

struct LightUniformPack
{
  GLint mPosLoc;    // Location of vec3 position.
  GLint mDirLoc;    // Location of vec3 direction.
  GLint mLdLoc;     // Location of diffuse component.
  GLint mLsLoc;     // Location of specular component.
  GLint mAlphaLoc;  // Location of alpha component.
};

struct LightSource
{
  // Position and direction (in world coordinates).
  glm::vec3 mPos;
  glm::vec3 mDir;

  // Phong illumination model data.
  glm::vec3 mLd;  // Diffuse component.
  glm::vec3 mLs;  // Specular component.
  GLfloat mAlpha; // Shininess.
};


}  // namespace gloo.