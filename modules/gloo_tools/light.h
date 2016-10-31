// + ======================================== +
// |         gl-oo-interface library          |
// |            Module: GLOO Tool.            |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +

#pragma once

#include <vector>
#include <initializer_list>

#define GLM_SWIZZLE
#include "gl_header.h"
#include "transform.h"

namespace gloo
{

struct LightUniformPack
{
  GLint mPosLoc;    // Location of vec3 position.
  GLint mDirLoc;    // Location of vec3 direction.
  GLint mLaLoc;     // Location of ambient component.
  GLint mLdLoc;     // Location of diffuse component.
  GLint mLsLoc;     // Location of specular component.
  GLint mAlphaLoc;  // Location of alpha component.
};

const GLint kNoUniform = -1;

class LightSource
{
public:
  // Constructor/Destructor.
  LightSource();
  virtual ~LightSource();

  // Add a rendering option for this light source.
  // Each option specifies all uniform locations for a rendering pass.
  int AddRenderingPass(const LightUniformPack & uniformPack);

  // Sets light uniforms in shader according to renderingPass.
  // Must be called before rendering objects in each pass.
  // By default, the position is specified in world coordinates!
  void SetLight(int renderingPass = 0) const;

  void SetLightInCameraCoordinates(const Transform & transform, int renderingPass = 0) const;

  // TODO.
  void ApplyTransformation(const Transform & transform);

  // Getters.
  inline glm::vec3 GetPosition()  const { return mPos; }
  inline glm::vec3 GetDirection() const { return mDir; }

  inline glm::vec3 GetAmbientComponent()  const { return mLa; }
  inline glm::vec3 GetDiffuseComponent()  const { return mLd; }
  inline glm::vec3 GetSpecularComponent() const { return mLs; }
  inline GLfloat GetShininess() const { return mAlpha; }

  // Setters.
  inline void SetPosition( const glm::vec3 & pos) { mPos = pos; }
  inline void SetDirection(const glm::vec3 & dir) { mDir = dir; }

  inline void SetAmbientComponent( const glm::vec3 & La) { mLa = La; }
  inline void SetDiffuseComponent( const glm::vec3 & Ld) { mLd = Ld; }
  inline void SetSpecularComponent(const glm::vec3 & Ls) { mLs = Ls; }

protected:
  // Position and direction (in world coordinates).
  glm::vec3 mPos { 0.0f, 0.0f, 0.0f };
  glm::vec3 mDir { 0.0f, 1.0f, 0.0f };

  // Phong illumination model data.
  glm::vec3 mLa { 0.1f, 0.1f, 0.1f };  // Ambient component.
  glm::vec3 mLd { 0.8f, 0.8f, 0.8f };  // Diffuse component.
  glm::vec3 mLs { 0.1f, 0.1f, 0.1f };  // Specular component.
  GLfloat mAlpha { 1.0f };  // Shininess.

  // List of rendering options for this light source.
  // Usually each shader has different pack of uniforms.
  std::vector<LightUniformPack> mUniformPackList;
};



}  // namespace gloo.