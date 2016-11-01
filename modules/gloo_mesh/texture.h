// + ======================================== +
// |         gl-oo-interface library          |
// |            Module: GLOO Mesh.            |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +

#pragma once

#include <string>
#include "imageIO.h"
#include "gloo/gl_header.h"

namespace gloo
{

class Texture2d
{
public:
  Texture2d()  { }
  ~Texture2d();

  void Bind(GLenum slot) const;

  bool Load(ImageIO* source);
  bool Load(int width, int height);
  bool Load(const std::string & filename);

private:
  GLuint mBuffer { 0 };  // Texture buffer object.
};

inline
void Texture2d::Bind(GLenum slot) const
{
  // TODO.
}


}  // namespace gloo.