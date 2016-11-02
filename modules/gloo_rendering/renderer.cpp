#include "renderer.h"

#include <iostream>

#define LOG_OUTPUT_ON 1

namespace gloo
{

// bool AddUniform(  const std::string & alias, const std::string & name);
// bool AddAttribute(const std::string & alias, const std::string & name);

bool RenderingPass::AddUniform(const std::string & name)
{
  if (mShaderProgram)
  {
    // Query location of uniform.
    GLint loc = mShaderProgram->GetUniformLocation(name);
    mUniformMap[name] = loc;

    // If it exists, return true.
    if (loc != -1)
    {
      return true;
    }
    else
    {
      return false;
    }
  }

#if LOG_OUTPUT_ON == 1
  std::cerr << "WARNING No shader program set." << std::endl;
#endif

  return false;
}

bool RenderingPass::AddAttribute(const std::string & name)
{
  if (mShaderProgram)
  {
    // Query location of attribute.
    GLint loc = mShaderProgram->GetAttribLocation(name);
    mAttributeMap[name] = loc;

    // If it exists, return true.
    if (loc != -1)
    {
      return true;
    }
    else
    {
      return false;
    }
  }

#if LOG_OUTPUT_ON == 1
  std::cerr << "WARNING No shader program set." << std::endl;
#endif

  return false;
}

void RenderingPass::ListUniforms() const
{
  std::cout << "List of uniforms: " << std::endl;

  if (mUniformMap.size() > 0)
  {
    for (const auto & uniform : mUniformMap)
    {
      std::cout << "\t" << uniform.first << " -> " << uniform.second << std::endl;
    }
  }
  else
  {
    std::cout << "\t <empty>" << std::endl;
  }

  std::cout << std::endl;
}

void RenderingPass::ListAttributes() const
{
  std::cout << "List of attributes: " << std::endl;

  if (mAttributeMap.size() > 0)
  {
    for (const auto & attribute : mAttributeMap)
    {
      std::cout << "\t" << attribute.first << " -> " << attribute.second << std::endl;
    }
  }
  else
  {
    std::cout << "\t <empty>" << std::endl;
  }

  std::cout << std::endl;
}

bool RenderingPass::AddUniform(  const std::string & alias, const std::string & name)
{
  // TODO.
}

bool RenderingPass::AddAttribute(const std::string & alias, const std::string & name)
{
  // TODO.
}


}  // namespace gloo.