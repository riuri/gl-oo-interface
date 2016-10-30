// + ======================================== +
// |         gl-oo-interface library          |
// |            Module: GLOO Tool.            |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +

#pragma once

#include <glm/glm.hpp>
#include "gl_header.h"

namespace gloo
{

struct LightUniformPack
{
  GLint mLaLoc;
  GLint mLdLoc;
  GLint mLsLoc;
  GLint mAlphaLoc;
};

class LightSource
{
public:
  // Constructor/Destructor.
  LightSource();
  virtual ~LightSource();

  // TODO: methods for handling uniforms.

  // Getters.
  glm::vec3 GetPosition()  const { return mPos; }
  glm::vec3 GetDirection() const { return mDir; }

  glm::vec3 GetAmbientComponent()  const { return mLa; }
  glm::vec3 GetDiffuseComponent()  const { return mLd; }
  glm::vec3 GetSpecularComponent() const { return mLs; }
  GLfloat GetShininess() const { return mAlpha; }

  // Setters.
  void SetPosition( const glm::vec3 & pos) { mPos = pos; } 
  void SetDirection(const glm::vec3 & dir) { mDir = dir; } 

  void SetAmbientComponent( const glm::vec3 & La) { mLa = La; } 
  void SetDiffuseComponent( const glm::vec3 & Ld) { mLd = Ld; } 
  void SetSpecularComponent(const glm::vec3 & Ls) { mLs = Ls; } 

protected:
  // Position and direction (in world coordinates).
  glm::vec3 mPos { 0.0f, 0.0f, 0.0f };
  glm::vec3 mDir { 0.0f, 0.0f, -1.f };

  // Phong illumination model data.
  glm::vec3 mLa { 0.1f, 0.1f, 0.1f };  // Ambient component.
  glm::vec3 mLd { 0.8f, 0.8f, 0.8f };  // Diffuse component.
  glm::vec3 mLs { 0.1f, 0.1f, 0.1f };  // Specular component.
  GLfloat mAlpha { 1.0f };  // Shininess.

  // TODO:  Add light uniform pack.
};



}  // namespace gloo.