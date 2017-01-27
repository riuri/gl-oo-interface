// + ======================================== +
// |         gl-oo-interface library          |
// |         Module: GLOO Rendering.          |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +

//  -------------------------------------------------------------------------------------------
//  Renderer is a base class for a general multi-pass rendering pipeline.
//  It provides an interface for rendering general scenes with light sources and objects.
//  gloo-rendering provides a few custom renderers. Please refer to:
//  
//  1. debug_renderer.h
//  2. phong_renderer.h
//  3. phong_shadow_mapping_renderer.h
//
//  The derived class should implement all methods in a custom way, according to each rendering
//  pass. For example, when you're implementing a shadow mapping with phong shading, you should
//  add two ShaderPrograms to handle both passes as well as frame buffer objects. For more det-
//  ails, please read gl-oo-interface derivations of gloo::Renderer.
//
//  In your derived class, you should add a custom Render() method to take your custom scene as
//  input (I recommend you to create a scene struct to pass all elements, including lights, ca-
//  mera and objects).
//
//  Recommended usage: construct, load, bind, get/set attributes, render.
//
//  -------------------------------------------------------------------------------------------

#pragma once

#include <vector>
#include <string>

#include "gloo/shader_program.h"

namespace gloo
{

class Renderer
{
public:
  // Constructor.
  Renderer()  { }

  // Destructor.
  virtual ~Renderer() { }

  // Binds the corresponding shader program no. 'renderinPass'.
  virtual void Bind(int renderingPass = 0) = 0;

  // Does setup of attributes.
  virtual bool Load() = 0;

  // Renders a scene containing a list of light sources and objects through the point of view
  // of camera.
  virtual void Render() const { }

  // Pure virtual getter and setter methods.
  virtual unsigned GetNumRenderingPasses() const = 0;
  virtual const ShaderProgram* GetShaderProgram(int renderingPass = 0) const = 0;

  // Get attribute/uniform for the corresponding rendering pass.
  virtual GLint GetAttribLocation( const std::string & name, int renderingPass = 0) const = 0;
  virtual GLint GetUniformLocation(const std::string & name, int renderingPass = 0) const = 0;
};

}  // namespace gloo.




