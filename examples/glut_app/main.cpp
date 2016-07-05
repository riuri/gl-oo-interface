#include <iostream>
#include <gloo/glut_application.h>

#include "my_model.h"

using namespace gloo;

int main(int argc, char* argv[]  )
{
  return GlutApplication::Run(argc, argv, new MyModel());
}