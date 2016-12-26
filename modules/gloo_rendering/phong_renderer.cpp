#include "phong_renderer.h"

#define LOG_OUTPUT_ON 1

namespace gloo
{

PhongRenderer::~PhongRenderer()
{
  delete mPhongShader;
}

void PhongRenderer::SetCamera(const Camera* camera) const
{
  camera->SetUniformProjMatrix(mProjMatrixLoc);
  camera->SetUniformViewMatrix(mViewMatrixLoc);
}

void PhongRenderer::SetModelNormalMatrix(const Transform & model) const
{
  model.SetUniform(mModelMatrixLoc);
  model.SetInverseTransposeUniform(mNormalMatrixLoc);
}

bool PhongRenderer::Load()
{
  // Allocate shader.
  mPhongShader = new ShaderProgram();

  // Load and build it.
  mPhongShader->BuildFromFiles(mVertexShaderPath, mFragmentShaderPath);

  // Check if compilation was successful.
  gloo::CompilationStatus status = mPhongShader->GetCompilationStatus();

  if (status == gloo::CompilationStatus::kSuccess)
  {
    mPhongShader->Bind();

    // Get uniform/attribute locations.
    mPositionAttribLoc = mPhongShader->GetAttribLocation("v_position");
    mNormalAttribLoc   = mPhongShader->GetAttribLocation("v_normal");
    mTextureAttribLoc  = mPhongShader->GetAttribLocation("v_uv");

    mProjMatrixLoc   = mPhongShader->GetUniformLocation("P");
    mViewMatrixLoc   = mPhongShader->GetUniformLocation("V");
    mModelMatrixLoc  = mPhongShader->GetUniformLocation("M");
    mNormalMatrixLoc = mPhongShader->GetUniformLocation("N");

    return true;
  }
  else 
  {
    return false;
  }
}

void PhongRenderer::Bind(int renderingPass)
{
  if (mPhongShader) 
  {
    mPhongShader->Bind();
  }
}

GLint PhongRenderer::GetAttribLocation(const std::string & name, int renderingPass) const
{
  if (name == "position" || name == "v_position")
  {
    return mPositionAttribLoc;
  }
  else if (name == "texture" || name == "v_uv" || name == "uv")
  {
    return mTextureAttribLoc;
  }
  else if (name == "normal" || name == "v_normal")
  {
    return mNormalAttribLoc;
  }
  else  // There is no other attributes.
  {
    return -1;
  }
}

GLint PhongRenderer::GetUniformLocation(const std::string & name, int renderingPass) const
{
  if (name == "M" || name == "model")
  {
    return mModelMatrixLoc;
  }
  else if (name == "V" || name == "view")
  {
    return mViewMatrixLoc;
  }
  else if (name == "P" || name == "projection" || name == "proj")
  {
    return mProjMatrixLoc;
  }
  else if (name == "N" || name == "normal")
  {
    return mNormalMatrixLoc;
  }
  
  else  // There is no other uniforms.
  {
    return -1;
  }
}


}  // namespace gloo.