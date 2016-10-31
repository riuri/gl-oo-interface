#include "light.h"

namespace gloo
{

int LightSource::AddRenderingPass(const LightUniformPack & uniformPack)
{
  mUniformPackList.push_back(uniformPack);

  return (mUniformPackList.size()-1);
}

namespace 
{
  void SetUniform3f(GLint loc, const glm::vec3 & data)
  {
    if (loc != kNoUniform)
    {
      glUniform3f(loc, data[0], data[1], data[2]);
    }
  }

  void SetUniform1f(GLint loc, GLfloat data)
  {
    if (loc != kNoUniform)
    {
      glUniform1f(loc, data);
    }
  }
}

void LightSource::SetLight(int renderingPass) const
{
  const LightUniformPack & uniformPack = mUniformPackList[renderingPass];

  SetUniform3f(uniformPack.mPosLoc,   mPos);
  SetUniform3f(uniformPack.mDirLoc,   mDir);
  SetUniform3f(uniformPack.mLaLoc,    mLa);
  SetUniform3f(uniformPack.mLdLoc,    mLd);
  SetUniform3f(uniformPack.mLsLoc,    mLs);
  SetUniform1f(uniformPack.mAlphaLoc, mAlpha);
}

void LightSource::SetLightInCameraCoordinates(const Transform & transform, int renderingPass) const
{
  const LightUniformPack & uniformPack = mUniformPackList[renderingPass];

  // Transform position/direction to camera coordinates.
  glm::vec4 p = glm::vec4(mPos, 1.0f);
  glm::vec4 d = glm::vec4(mDir, 1.0f);
  const glm::mat4 & V = transform.GetMatrix();

  p = V * p;
  d = V * d;
  p = p / p[3];  // Normalize homogenous coordinates.

  // Set block of uniforms.
  SetUniform3f(uniformPack.mPosLoc,   p.xyz());
  SetUniform3f(uniformPack.mDirLoc,   d.xyz());
  SetUniform3f(uniformPack.mLaLoc,    mLa);
  SetUniform3f(uniformPack.mLdLoc,    mLd);
  SetUniform3f(uniformPack.mLsLoc,    mLs);
  SetUniform1f(uniformPack.mAlphaLoc, mAlpha);
}

void LightSource::ApplyTransformation(const Transform & transform)
{
  glm::vec4 p = glm::vec4(mPos, 1.0f);
  glm::vec4 d = glm::vec4(mDir, 1.0f);
  const glm::mat4 & V = transform.GetMatrix();

  p = V * p;
  d = V * d;
  p = p / p[3];  // Normalize homogenous coordinates.

  mPos = p.xyz();
  mDir = d.xyz();
}

}  // namespace gloo.
