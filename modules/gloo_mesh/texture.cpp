#include "texture.h"

namespace gloo
{

Texture2d::~Texture2d()
{
  glDeleteTextures(1, &mBuffer);
}

bool Texture2d::Load(ImageIO* source)
{
  // TODO.
}

bool Texture2d::Load(int width, int height)
{
  // glGenTexture(1, &mBuffer);
}

bool Texture2d::Load(const std::string & filename)
{
  // TODO.
}


}  // namespace gloo.