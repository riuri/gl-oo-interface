// + ======================================== +
// |         gl-oo-interface library          |
// |        Module: GLOO Shader.              |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +
//
//  TODO: description here.
//
//
//
//
//
//
//
//
//
//
//
//
// 

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
                      const char* geometryShderPath                = nullptr,
                      const char* tesselationControlShaderPath     = nullptr,
                      const char* tessellationEvaluationShaderPath = nullptr);

  bool BuildFromFiles(const std::string & vertexShaderPath, 
                      const std::string & fragmentShaderPath,
                      const std::string & geometryShderPath                = "",
                      const std::string & tesselationControlShaderPath     = "", 
                      const std::string & tessellationEvaluationShaderPath = "");


  // load shaders from a C text string

  GLuint GetHandle() { return mHandle; }

protected:

  GLuint mHandle { 0 };  // OpenGL handle for the entire shader program.



};



}  // namespace gloo.