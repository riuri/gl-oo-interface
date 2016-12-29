#include "texture.h"

#include <iostream>

#define LOG_OUTPUT_ON 1

namespace gloo
{

Texture2d::~Texture2d()
{
  glDeleteTextures(1, &mBuffer);
}

bool Texture2d::Load(ImageIO* source, GLenum format, GLenum type)
{
  glGenTextures(1, &mBuffer);
  glBindTexture(GL_TEXTURE_2D, mBuffer);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int bytesPerPixel = source->getBytesPerPixel();
  // TODO: use bytesPerPixel to use a different input internal format.

  glTexImage2D( GL_TEXTURE_2D,  // Target.
                0,              // Detail level - original.
                GL_RGBA,        // How the colors are stored.
                source->getWidth(),   // Width.
                source->getHeight(),  // Height.
                0,                    // Border must be 0. 
                format,               // Input format.  
                type,                 // Input data type.
                source->getPixels()   // Buffer address.
  );

  return true;
}

bool Texture2d::Load(int width, int height, GLenum format, GLenum type)
{
  glGenTextures(1, &mBuffer);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mBuffer);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D( GL_TEXTURE_2D,  // Target.
                0,              // Detail level - original.
                GL_RGBA,        // How many channels the texture will have.
                width,   // Width.
                height,  // Height.
                0,        // Border must be 0. 
                format,   // Format (RGB, RGBA, GRBA, and so on).  
                type,     // Data type (unsigned byte, ...).
                nullptr   // Buffer address.
              );

  return true;
}

bool Texture2d::Load(const std::string & filename, GLenum format, GLenum type)
{
  bool successful = false;
  ImageIO* source = new ImageIO();
  if (source->loadJPEG(filename.c_str()) == ImageIO::OK)
  {
    Texture2d::Load(source, format, type);
    successful = true;
  }
  else
  {
#if LOG_OUTPUT_ON == 1
    std::cerr << "WARNING Texture file at " << filename << " could not be loaded.\n";
#endif
  }

  delete source;
  return successful;
}


}  // namespace gloo.