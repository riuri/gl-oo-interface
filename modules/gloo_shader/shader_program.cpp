#include "shader_program.h"
#include <iostream>

namespace gloo
{

bool ShaderProgram::BuildFromFiles(const char* vertexShaderPath, 
                                   const char* fragmentShaderPath,
                                   const char* geometryShaderPath,
                                   const char* tessellationControlShaderPath,
                                   const char* tessellationEvaluationShaderPath)
{
#if LOG_OUTPUT_ON == 1
  std::cout << "-- BUILDING Shaders and LINKING them to OpenGL --" << std::endl;
#endif


  char * shaderCodes[5] = { NULL, NULL, NULL, NULL, NULL };
  const char * filenames[5] = { vertexShaderPath, fragmentShaderPath, geometryShaderPath, 
                                tessellationControlShaderPath, tessellationEvaluationShaderPath };

  for (int i = 0; i < 5; i++) 
  {
    // If filename not provided, skip that shader.
    if (filenames[i] == NULL) 
    {
      shaderCodes[i] = NULL;
      continue;
    }

    // Allocate space for shader code (128 Kb -- should be enough).
    shaderCodes[i] = new char[128 * 1024];

    // Load the shader into the shaderCodes string.
    if (LoadShader(filenames[i], shaderCodes[i], 128 * 1024) != 0) 
    {
#if LOG_OUTPUT_ON == 1
      std::cout << "ERRROR: Shader " << filenames[i] << " file not found." << std::endl;
#endif
      return 1;
    }
  }

  int exitCode = BuildFromStrings(shaderCodes[0], shaderCodes[1], shaderCodes[2], shaderCodes[3], shaderCodes[4]);
  for (int i = 0; i < 5; i++) 
  {
    delete [] (shaderCodes[i]);
    shaderCodes[i] = NULL;
  }

  return exitCode;
}

bool ShaderProgram::BuildFromFiles(const std::string & vertexShaderPath, 
                                   const std::string & fragmentShaderPath,
                                   const std::string & geometryShaderPath,
                                   const std::string & tessellationControlShaderPath, 
                                   const std::string & tessellationEvaluationShaderPath)
{
  const char* geometryShaderPathBuffer = NULL;
  const char* tessellationControlShaderPathBuffer   = NULL;
  const char* tessellationEvaluationShaderPathBuffer = NULL;

  if (geometryShaderPath.size() > 0)
  {
    geometryShaderPathBuffer = geometryShaderPath.c_str();
  }
  if (tessellationEvaluationShaderPath.size() > 0)
  {
    tessellationEvaluationShaderPathBuffer = tessellationEvaluationShaderPath.c_str();
  }
  if (tessellationControlShaderPath.size() > 0)
  {
    tessellationControlShaderPathBuffer = tessellationControlShaderPath.c_str();
  }

  return ShaderProgram::BuildFromFiles(vertexShaderPath.c_str(),
                                       fragmentShaderPath.c_str());
}

bool ShaderProgram::BuildFromStrings(const char* vertexShaderCode, 
                                     const char* fragmentShaderCode,
                                     const char* geometryShaderCode,
                                     const char* tessellationControlShaderCode,
                                     const char* tessellationEvaluationShaderCode)
{
  // Create an overall program handle.
  mHandle = glCreateProgram();
  if (mHandle == 0) 
  {
    std::cerr << "ERROR: Shader Program initialization failed." << std::endl;
    return -1;
  }

  // Store the codes into one array.
  const char * shaderCode[5] = { vertexShaderCode, fragmentShaderCode, geometryShaderCode, 
                                 tessellationControlShaderCode, tessellationEvaluationShaderCode };

  GLuint h_shaders[5] = { 0, 0, 0, 0, 0 }; // Shader handles to-be-created.
  
  // OpenGL shader flags (macros are used to prevent a compile error in case the OpenGL 
  // version is too low and a symbolic constant is not defined).
  GLenum shaderFlags[5] = 
  {
    GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, 
    #if defined(GL_GEOMETRY_SHADER)
      GL_GEOMETRY_SHADER,
    #else
      0,
    #endif
    #if defined(GL_TESS_CONTROL_SHADER) && defined(GL_TESS_EVALUATION_SHADER)
       GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER
    #else
      0, 0
    #endif
  };

  // informative shader names
  std::vector<std::string> shaderName = { "Vertex shader", "Fragment shader", "Geometry shader", 
                              "Tessellation control shader", "Tessellation evaluation shader" };

  for (int i = 0; i < 5; i++)
  {
    // If code is not provided, skip this shader.
    if (shaderCode[i] == NULL)
      continue;

#if LOG_OUTPUT_ON == 1
    std::cout << "Compiling " << shaderName[i] << "..." << std::endl;
#endif

    // compile the shader
    if (CompileShader(shaderCode[i], shaderFlags[i], h_shaders[i]) != 0)
    {
      std::string infoLogStr = "In shader " + std::string(shaderName[i]);
      mCompilationLog.push_back(infoLogStr);  // Save infoLog.

#if LOG_OUTPUT_ON == 1
      std::cout << "COMPILE ERROR: shader " << shaderName[i] << std::endl;
#endif
      
      return 1;
    }
    else 
    {
      // Attach the shader to the pipeline program
      glAttachShader(mHandle, h_shaders[i]);
    }
  }

  // Link.
  glLinkProgram(mHandle);
  
  int status;
  glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
  if (status == 0)
  {
    GLchar infoLog[512];
    glGetProgramInfoLog(mHandle, 512, NULL, infoLog);
    mCompilationLog.emplace_back(&infoLog[0]);  // Save infoLog.

#if LOG_OUTPUT_ON == 1
    std::cout << "LINKER ERROR:\n" << infoLog << std::endl;
#endif

    return 1;
  }

  // The shaders are no longer needed after the program is linked.
  for (int i = 0; i < 5; i++)
    glDeleteShader(h_shaders[i]);

  return 0;

  return false;
}


int ShaderProgram::CompileShader(const char * shaderCode, GLenum shaderType, GLuint & shaderHandle)
{
  shaderHandle = glCreateShader(shaderType);

  if (shaderHandle == 0) 
  {
#if LOG_OUTPUT_ON == 1
    std::cerr << "ERROR: Creation of shader failed." << std::endl;
#endif
    return 1;
  }

  const int numShaderCodes = 1;
  const GLchar * shaderCodes[] = { shaderCode };
  GLint codeLength[] = { (GLint)strlen(shaderCode) };

  // Compile code.
  glShaderSource(shaderHandle, numShaderCodes, shaderCodes, codeLength);
  glCompileShader(shaderHandle);

  // Check if compilation was successful.
  GLint status;
  glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);
  if (status == 0)  // Not successful.
  {
    GLchar infoLog[512];
    glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);
    std::string infoLogStr(&infoLog[0]);
    //infoLogStr = "COMPILE ERROR: " + infoLogStr; 
    mCompilationLog.emplace_back(&infoLog[0]);  // Save infoLog.

#if LOG_OUTPUT_ON == 1
    std::cerr << "COMPILE ERROR: \n" << infoLog << std::endl;
#endif

    return 1;
  }

  return 0;
}

int ShaderProgram::LoadShader(const char * filename, char * code, int len)
{
#if LOG_OUTPUT_ON == 1
    std::cout << "Loading shader from " << filename << std::endl;
#endif

  FILE * file = fopen(filename, "rb");
  if (file == NULL)
    return 1;

  code[fread(code, 1, len, file)] = '\0';
  fclose(file);

#if LOG_OUTPUT_ON == 1
    std::cout << "Successfully loaded." << std::endl << std::endl;
#endif


  return 0;
}

GLuint ShaderProgram::GetVariableHandle(const char * variableName) const
{ 
   GLint vHandle = glGetUniformLocation(mHandle, variableName);

#if LOG_OUTPUT_ON == 1
  if (vHandle == -1)
  {
    std::cout << "Variable not found \'" << variableName << '\'' << std::endl;
  }
#endif
    
  return vHandle;
}

GLuint ShaderProgram::GetVariableHandle(const std::string & variableName) const
{
  return GetVariableHandle(variableName.c_str());
}

void ShaderProgram::PrintCompilationLog()
{
  for (int i = 0; i < mCompilationLog.size(); i++)
  {
    std::cout << i+1 << ". " << mCompilationLog[i];
  }
  std::cout << std::endl;
}

} // namespace gloo.