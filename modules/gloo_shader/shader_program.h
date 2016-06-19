// + ======================================== +
// |         gl-oo-interface library          |
// |        Module: GLOO ShaderProgram.       |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +
//
//  gloo::ShaderProgram implements a shader program of
//  shader-based OpenGL pipeline.
//
//  It is strongly based on the starter code 
//  provided by Professor Jernej Barbic and his
//  TA Bohan Wang for CSCI 420 at USC. I adapted
//  for an easier and more general use.
//
//  INSTRUCTIONS
//
//  Initialization/Compilation:
//  1. Create an instance:                gloo::ShaderProgram* program = new gloo::ShaderProgram();
//  2. Build it fromcode files:           program->BuildFromFiles(vtxPath, gmtryPath, ...);
//  3. Link when necessary (before use):  program->Bind();
//
//  How to check for compilation status?
//  gloo::CompilationStatus status = program->GetCompilationStatus();
//    -> kError:   compilation error
//    -> kSuccess: successfully compiled and linked
//    -> kLinkError: linking error (e.g. main not found)
//    -> kLoadFailure: a provided file doesn't have the correct path/couldn't be opened.
// 
//  Get log message (errors):
//  std::vector<std::string> log = program->GetCompilationLog();
//
//  Getting handle for variables by the name:
//  GLuint h_modelView = program->GetVariableHandle("MV");

#pragma once

#include "gl_header.h"

#include <vector>
#include <string>

namespace gloo
{

enum CompilationStatus { kSuccess, kError, kLinkError, kLoadFailure, kUnitialized };

class ShaderProgram
{
public:
  ShaderProgram() { } 

  ~ShaderProgram() 
  { 
    glDeleteProgram(mHandle);
  }

  // Loads shaders from files specified by the corresponding paths.
  bool BuildFromFiles(const char* vertexShaderPath, 
                      const char* fragmentShaderPath,
                      const char* geometryShaderPath               = nullptr,
                      const char* tesselationControlShaderPath     = nullptr,
                      const char* tessellationEvaluationShaderPath = nullptr);

  bool BuildFromFiles(const std::string & vertexShaderPath, 
                      const std::string & fragmentShaderPath,
                      const std::string & geometrySahderPath               = "",
                      const std::string & tessellationControlShaderPath    = "", 
                      const std::string & tessellationEvaluationShaderPath = "");

  // Loads shaders from buffer in memory (c-string).
  bool BuildFromStrings(const char* vertexShaderCode, 
                        const char* fragmentShaderCode,
                        const char* geometryShaderCode               = nullptr,
                        const char* tessellationControlShaderCode    = nullptr,
                        const char* tessellationEvaluationShaderCode = nullptr);


  // Binds this shader program as the currrent renderer shader.
  inline void Bind() { glUseProgram(mHandle); }

  // Returns shader program handle.
  inline GLuint GetHandle() const { return mHandle; }
  
  // Returns the handle for a variable stored into this shader program.
  GLuint GetVariableHandle(const char * variableName) const;
  GLuint GetVariableHandle(const std::string & variableName) const;

  // Returns the vector of compilation messages (as a copy).
  std::vector<std::string> GetCompilationLog() { return mCompilationLog; }

  // Returns compilation status - kError, kLinkError, kLoadFailure, kSuccess if BuildFromFiles was called.
  // It returns kUnitilized if it wasn't built.
  CompilationStatus GetCompilationStatus() { return mCompilationStatus; }

  // Shows the compilate error log on the console output.
  void PrintCompilationLog();

  // Compiles shader code stored in buffer shaderCode.
  //   shaderCode: the shader code.
  //   shaderType: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER, 
  //               GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER.
  // Output:
  //   shaderHandle: the handle to the compiled shader.
  //   return value: 0=success, non-zero: failure.
  int CompileShader(const char* shaderCode, GLenum shaderType, GLuint & shaderHandle);

  // Loads shader code from file and stores into code buffer.
  int LoadShader(const char* filename, char* code, int len);

protected:
  GLuint mHandle { 0 };  // OpenGL handle for the entire shader program.

  CompilationStatus mCompilationStatus { kUnitialized };  // Tells the result of compilation (see enum).
  std::vector<std::string> mCompilationLog;               // Stores all error messages from compiler/linker.
};

}  // namespace gloo.