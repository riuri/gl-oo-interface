// + ======================================== +
// |         gl-oo-interface library          |
// |        Module: GLOO Shader.              |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +
//
//  gloo::Shader implements a shader program of
//  shader-based OpenGL pipeline.
//
//  It is strongly based on the starter code 
//  provided by Professor Jernej Barbic and his
//  TA Bohan Wang for CSCI 420 at USC. 
//  
//  Usage:
//  1. Create an instance:                 gloo::Shader* shader = new gloo::Shader();
//  2. Build it fromcode files:            shader->BuildFromFiles(vtxPath, gmtryPath, ...);
//  3. Link when necessary (before use):   shader->Bind();
//  
//  To get name or set variables:
//  TODO(RC): Description.

#pragma once

#define LOG_ON 0

#include "gl_header.h"
#include <string>

namespace gloo
{

class Shader
{
public:
  Shader();
  virtual ~Shader();

  // Loads shaders from files specified by the corresponding paths.
  bool BuildFromFiles(const char* vertexShaderPath, 
                      const char* fragmentShaderPath,
                      const char* geometryShaderPath               = nullptr,
                      const char* tesselationControlShaderPath     = nullptr,
                      const char* tessellationEvaluationShaderPath = nullptr);

  bool BuildFromFiles(const std::string & vertexShaderPath, 
                      const std::string & fragmentShaderPath,
                      const std::string & geometrySahderPath               = "",
                      const std::string & tesselationControlShaderPath     = "", 
                      const std::string & tessellationEvaluationShaderPath = "");

  // Loads shaders from buffer in memory.
  bool BuildFromStrings(const char* vertexShaderCode, 
                        const char* fragmentShaderCode,
                        const char* geometryShaderCode               = nullptr,
                        const char* tesselationControlShaderCode     = nullptr,
                        const char* tessellationEvaluationShaderCode = nullptr);


  void Bind();
  inline GLuint GetHandle() { return mHandle; }

protected:

  GLuint mHandle { 0 };  // OpenGL handle for the entire shader program.



};



}  // namespace gloo.