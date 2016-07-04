#include <iostream>
#include <gloo/glut_application.h>

#include "my_model.h"

using namespace gloo;

int main(int argc, char* argv[]  )
{
  GlutApplication::Init(argc, argv, new MyModel());
  GlutApplication::Run();
  GlutApplication::Close();

  return 0;
}