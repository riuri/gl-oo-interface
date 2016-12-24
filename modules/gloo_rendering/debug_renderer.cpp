#include "debug_renderer.h"

#define LOG_OUTPUT_ON 1

namespace gloo
{

DebugRenderer::~DebugRenderer()
{
  delete mDebugShader;
}

void DebugRenderer::Bind(int renderingPass) 
{ 
  if (mDebugShader)
  {
    mDebugShader->Bind();
  }
  // ELSE: show an error?
}

bool DebugRenderer::Load()
{
  // Allocate shader program.
  mDebugShader = new ShaderProgram();

  // Load and build it.
  mDebugShader->BuildFromFiles(mVertexShaderPath, mFragmentShaderPath);

  // Check if compilation was successful.
  gloo::CompilationStatus status = mDebugShader->GetCompilationStatus();

  if (status == gloo::CompilationStatus::kSuccess)
  {
    mDebugShader->Bind();

    // Get main attribute/uniform locations in advance.
    mPositionAttribLoc = mDebugShader->GetAttribLocation("v_position");
    mColorAttribLoc    = mDebugShader->GetAttribLocation("v_color");
    mModelViewProjMatrixLoc = mDebugShader->GetUniformLocation("MVP");

    return true;
  }
  else
  {
    return false;
  }
}

GLint DebugRenderer::GetAttribLocation(const std::string & name, int renderingPass) const
{
  if (name == "position" || name == "v_position")
  {
    return mPositionAttribLoc;
  }
  else if (name == "color" || name == "v_color")
  {
    return mColorAttribLoc;
  }
  else  // There is no other attributes.
  {
    return -1;
  }
}

GLint DebugRenderer::GetUniformLocation(const std::string & name, int renderingPass) const
{
  if (name == "MVP" || name == "model-view-projection" || name == "mvp")
  {
    return mModelViewProjMatrixLoc;
  }
  else  // There is no other uniforms.
  {
    return -1;
  }
}


}  // namespace gloo.