// + ======================================== +
// |         gl-oo-interface library          |
// |         Module: GLOO Rendering.          |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +

// TODO(Castiel): documentation.

#pragma once

#include "renderer.h"

namespace gloo
{

class DebugRenderer : public Renderer
{
public:
  DebugRenderer(const std::string & vertexShaderPath, 
                const std::string & fragmentShaderPath)
  : Renderer()
  , mVertexShaderPath(vertexShaderPath)
  , mFragmentShaderPath(fragmentShaderPath)
  { }

  // Default constructor.
  DebugRenderer()
  : Renderer()
  , mVertexShaderPath(  "../../shaders/debug/vertex_shader.glsl")
  , mFragmentShaderPath("../../shaders/debug/fragment_shader.glsl")
  { }

  ~DebugRenderer();

  bool Load();
  void Render(/* TODO: light sources, objects, etc */);

  inline unsigned GetNumRenderingPasses() const { return 1; }

  inline const ShaderProgram* GetShaderProgram(int renderingPass = 0) const { return mDebugShader; }
  
  GLint GetAttribLocation( const std::string & name, int renderingPass = 0) const;
  GLint GetUniformLocation(const std::string & name, int renderingPass = 0) const;

  // Extra methods.
  GLint GetPositionAttribLoc() const { return mPositionAttribLoc; }
  GLint GetColorAttribLoc()    const { return mColorAttribLoc;    }
  GLint GetModelViewProjUniformLoc() const { return mModelViewProjMatrixLoc; }

protected:
  // Shader program.
  ShaderProgram* mDebugShader { nullptr };

  // Main attribute/uniform locations.
  GLint mPositionAttribLoc { -1 };
  GLint mColorAttribLoc    { -1 };
  GLint mModelViewProjMatrixLoc { -1 };

  // Constant data (passed to constructor).
  const std::string mVertexShaderPath;
  const std::string mFragmentShaderPath;
};


}  // namespace gloo.