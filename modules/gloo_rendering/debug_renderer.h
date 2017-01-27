// + ======================================== +
// |         gl-oo-interface library          |
// |         Module: GLOO Rendering.          |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +

// ------------------------------------------------------------------------------------------------
// DebugRenderer is a single-pass renderer for debug meshes that don't require illumination.
// It can also render wireframes or "material-less/texture-less" objects.
// Internally, it loads the following shaders by default: 
//    gl-oo-interface/shaders/debug/vertex_shader.glsl
//    gl-oo-interface/shaders/debug/fragment_shader.glsl
//
// These shaders are very simple and optimized, and have only two attributes:
//   -> Vertex coordinates (3d) [use GetPositionAttribLoc()]. 
//   -> Vertex RGB color   (3d) [use GetColorAttribLoc()].
// and only one uniform:
//   -> Model-view-proj matrix (4x4) [use GetModelViewProjUniformLoc()].
//
// I strongly suggest that you use DebugRenderer to render useful meshes, such as Axis or Grid.
// This is a good way of getting feedback when programming complex hierachy of objects and so on.
//
// Usage:
//
// 1. Construct:
//  Default: DebugRenderer* mDebugRenderer = new DebugRenderer();
//  Custom:  DebugRenderer* mDebugRenderer = new DebugRenderer(vtxShaderPath, fragShaderPath);
//
// 2. Load and check for errors:
//  bool success = mDebugRenderer->Load();
//
// 3. Get shader default attribute/uniform locations:
//  GLint posAttribLoc  = mDebugRenderer->GetPositionAttribLoc();
//  GLint colAttribLoc  = mDebugRenderer->GetColorAttribLoc();
//  ...
//
// 4. Get custom attribute/uniform locations by name (in case you modified):
//  GLint customAttribLoc  = mDebugRenderer->GetAttribLocation( "<name>");
//  GLint customUniformLoc = mDebugRenderer->GetUniformLocation("<name>");
//  ...
//
// 5. Before use (i.e. rendering calls):
//  mDebugRenderer->Bind();
//
// 6. (optionally) For rendering mesh groups:
//  mDebugRenderer->Render(meshGroup, modelTransformation, camera);
//
// ------------------------------------------------------------------------------------------------

#pragma once

#include "renderer.h"
#include "gloo/group.h"
#include "gloo/camera.h"

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
