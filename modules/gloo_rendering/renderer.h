// + ======================================== +
// |         gl-oo-interface library          |
// |         Module: GLOO Rendering.          |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +

//  TODO(Castiel): write documentation.

#pragma once

#include "gloo/shader_program.h"

#include <string>
#include <unordered_map>

namespace gloo
{

// ====== RenderingPass ====== //

using StrIntMap  = std::unordered_map<std::string, int>;

class RenderingPass
{
public:
  // Constructor/Destructors.
  RenderingPass() { }
  virtual ~RenderingPass() { }

  // Getter/Setters.
  ShaderProgram* GetShaderProgram() { return mShaderProgram; }
  const ShaderProgram* GetShaderProgram() const { return mShaderProgram; }

  void SetShaderProgram(ShaderProgram* shaderProgram) { mShaderProgram = shaderProgram; }

  // Query methods for uniform/attribute locations.
  virtual GLint Uniform(  const std::string & alias) { return mUniformMap[alias];   }
  virtual GLint Attribute(const std::string & alias) { return mAttributeMap[alias]; }

  // Prints all uniforms/attributes.
  void ListUniforms() const;
  void ListAttributes() const;

  // Adds a new uniform/attribute (to make easier getting locations).
  // You can later on call Attribute() or Uniform() to get its location.
  // It automatically searches for its location in GPU and stores into the corresponding map.
  bool AddUniform(  const std::string & name);
  bool AddAttribute(const std::string & name);

  // Adds the uniform/attribute defined by 'name' to be referred by 'alias'.
  // It automatically searches for its location in GPU and stores into the corresponding map.
  bool AddUniform(  const std::string & alias, const std::string & name);
  bool AddAttribute(const std::string & alias, const std::string & name);

protected:
  /* Properties */
  ShaderProgram* mShaderProgram { nullptr };  // Pipeline shader program.

  StrIntMap mUniformMap;     // Unordered map [name] -> [loc] of uniforms.
  StrIntMap mAttributeMap;   // Unordered map [name] -> [loc] of attributes.
  
};

// ====== Renderer ====== // 

// class Renderer
// {
// public:

//   bool AddRenderingPass(RenderingPass* renderingPass);

// protected:
//   std::vector<RenderingPass*> mRenderingPassList;
// };


}  // namespace gloo.