// + ======================================== +
// |         gl-oo-interface library          |
// |         Module: GLOO Rendering.          |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +

// TODO(Castiel): documentation.

#pragma once

#include "renderer.h"
#include "gloo/group.h"
#include "gloo/camera.h"

#include <utility>

namespace gloo
{

struct DebugScene
{
  Camera* mCamera;
  std::vector<std::pair<MeshGroup<Batch>*, Transform*>>      mBatchObjectList;
  std::vector<std::pair<MeshGroup<Interleave>*, Transform*>> mInterleaveObjectList;
};

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

  // Renders the entire scene at once.
  void Render(const DebugScene* scene) const;

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


