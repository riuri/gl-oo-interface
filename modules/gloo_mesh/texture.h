// + ======================================== +
// |         gl-oo-interface library          |
// |            Module: GLOO Mesh.            |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +

#pragma once

#include <string>
#include "../../dependencies/imageIO/imageIO.h"
#include "gloo/gl_header.h"

namespace gloo
{

class Texture2d
{
public:
  Texture2d()  { }
  ~Texture2d();

  void Bind(GLenum slot=0) const;

  // Loads image source from buffer on memory.
  bool Load(ImageIO* source, GLenum format=GL_RGB, GLenum type=GL_UNSIGNED_BYTE);

  // Loads image from the disk at 'filename'.
  bool Load(const std::string & filename, GLenum format=GL_RGB, GLenum type=GL_UNSIGNED_BYTE);

  // Loads a non-initialized buffer.
  bool Load(int width, int height, GLenum format=GL_RGB, GLenum type=GL_UNSIGNED_BYTE);

private:
  GLuint mBuffer { 0 };  // Texture buffer object.
};

inline
void Texture2d::Bind(GLenum slot) const
{
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, mBuffer);
}


}  // namespace gloo.