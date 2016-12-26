// + ======================================== +
// |         gl-oo-interface library          |
// |         Module: GLOO Rendering.          |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +

// ------------------------------------------------------------------------------------------------
// DebugRenderer is a single-pass renderer for debug meshes that don't require illumination.
// It can also render wireframes or "material-less/texture-less" objects.
// Internally, it loads the following shaders: 
//    gl-oo-interface/shaders/debug/vertex_shader.glsl
//    gl-oo-interface/shaders/debug/fragment_shader.glsl
//
// These shaders are very simple and optimized, and have only two attributes:
//   -> Vertex coordinates (3d) [use GetPositionAttribLoc()]. 
//   -> Vertex RGB color (3d)   [use GetColorAttribLoc()].
// and only one uniform:
//   -> Model-view-proj matrix (4x4) [use GetModelViewProjUniformLoc()].
// ------------------------------------------------------------------------------------------------

#pragma once

#include "renderer.h"
#include "gloo/group.h"
#include "gloo/camera.h"

#include <utility>

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

  virtual void Bind(int renderingPass = 0);

  // Renders a specific object through a point of view.
  template <StorageFormat F>
  void Render(const MeshGroup<F>* mesh, Transform & model, Camera* camera, int pass=0) const;

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
  // It works as fast access variables (without querying the GPU).
  GLint mPositionAttribLoc { -1 };
  GLint mColorAttribLoc    { -1 };
  GLint mModelViewProjMatrixLoc { -1 };

  // Constant data (passed to constructor).
  const std::string mVertexShaderPath;
  const std::string mFragmentShaderPath;
};

template <StorageFormat F>
void DebugRenderer::Render(const MeshGroup<F>* mesh, Transform & model, Camera* camera, int pass) const
{
  camera->SetUniformModelViewProj(mModelViewProjMatrixLoc, model);  // Proj * View * Model.
  mesh->Render(pass);
}

}  // namespace gloo.
