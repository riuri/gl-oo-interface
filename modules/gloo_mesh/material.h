// + ======================================== +
// |         gl-oo-interface library          |
// |            Module: GLOO Mesh.            |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +

#pragma once

#include "gloo/gl_header.h"

namespace gloo
{

struct MaterialUniformPack
{
  GLint mKaLoc;  // Ambient component uniform loc.
  GLint mKdLoc;  // Diffuse component uniform loc.
  GLint mKsLoc;  // Specular component uniform loc.
};

struct Material
{
  glm::vec3 mKa;  // Ambient component.
  glm::vec3 mKd;  // Diffuse component.
  glm::vec3 mKs;  // Specular component.
};

}  // namespace gloo.