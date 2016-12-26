// + ======================================== +
// |         gl-oo-interface library          |
// |         Module: GLOO Rendering.          |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +

#pragma once 

#include "renderer.h"
#include "gloo/group.h"
#include "gloo/camera.h"

namespace gloo 
{

class PhongRenderer : public Renderer
{
public:
  PhongRenderer(const std::string & vertexShaderPath,
                const std::string & fragmentShaderPath)
  : Renderer()
  , mVertexShaderPath(vertexShaderPath)
  , mFragmentShaderPath(fragmentShaderPath)
  { }

  PhongRenderer()
  : Renderer()
  , mVertexShaderPath(  "../../shaders/phong/vertex_shader.glsl")
  , mFragmentShaderPath("../../shaders/phong/fragment_shader.glsl")
  { }

  ~PhongRenderer();

  bool Load();

  // If you're lazy to manually set the camera and then render the mesh, just call this method.
  // Please notice that by setting the camera before every object rendering, you will be updating
  // the M, V, P matrices without really needing.
  template <StorageFormat F>
  void Render(const MeshGroup<F>* mesh, Transform & model, Camera* camera, int pass=0) const;

  virtual void Bind(int renderingPass = 0);

  inline unsigned GetNumRenderingPasses() const { return 1; }
  inline const ShaderProgram* GetShaderProgram(int renderingPass = 0) const { return mPhongShader; }

  GLint GetAttribLocation( const std::string & name, int renderingPass = 0) const;
  GLint GetUniformLocation(const std::string & name, int renderingPass = 0) const;


  // Extra methods (fast access).
  GLint GetPositionAttribLoc() const { return mPositionAttribLoc; }
  GLint GetTextureAttribLoc()  const { return mTextureAttribLoc;  }
  GLint GetNormalAttribLoc()   const { return mNormalAttribLoc;   }

  GLint GetViewUniformLoc()   const { return mViewMatrixLoc; }
  GLint GetProjUniformLoc()   const { return mProjMatrixLoc; }
  GLint GetModelUniformLoc()  const { return mModelMatrixLoc;  }
  GLint GetNormalUniformLoc() const { return mNormalMatrixLoc; }

  // Auxiliar methods.
  void SetCamera(const Camera* camera) const;
  void SetModelNormalMatrix(const Transform & model) const;

private:
  // Shader Program.
  ShaderProgram* mPhongShader { nullptr };

  // Add fast-access uniform/attribute locations here...
  GLint mPositionAttribLoc { -1 };
  GLint mTextureAttribLoc  { -1 };
  GLint mNormalAttribLoc   { -1 };

  GLint mViewMatrixLoc   { -1 };
  GLint mProjMatrixLoc   { -1 };
  GLint mModelMatrixLoc  { -1 };
  GLint mNormalMatrixLoc { -1 };

  // even more down here...

  // Constant data (passed to constructor).
  const std::string mVertexShaderPath;
  const std::string mFragmentShaderPath;
};

template <StorageFormat F>
void PhongRenderer::Render(const MeshGroup<F>* mesh, Transform & model, Camera* camera, int pass) const
{
  model.SetUniform(mModelMatrixLoc);
  camera->SetUniformViewMatrix(mViewMatrixLoc);
  mesh->Render(pass);
}

}  // namespace gloo.