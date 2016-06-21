#pragma once
#include <cstring>

// Headers for shader-based OpenGL.
#if defined(WIN32) || defined(linux)
  #include <GL/glew.h>
#elif defined(__APPLE__)
  #include <OpenGL/gl3.h>
  #include <OpenGL/gl3ext.h>
#endif

// Headers for GLUT.
#if defined(WIN32) || defined(linux)
  #include <GL/glut.h>
  //#include <GL/freeglut_ext.h>
#elif defined(__APPLE__)
  #define __gl_h_
  #include <GLUT/glut.h>
#endif
