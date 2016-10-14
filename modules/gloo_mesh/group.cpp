#include "group.h"

namespace gloo
{

template <>
bool StaticGroup<Interleave>::Load(GLuint programHandle,
                                   const GLfloat* positions, 
                                   const GLfloat* colors,
                                   const GLfloat* normals,
                                   const GLfloat* uv,
                                   const GLuint* indices,
                                   int numVertices,
                                   int numIndices,
                                   GLenum drawMode)
{

}


template <>
bool StaticGroup<Batch>::Load(GLuint programHandle,
                              const GLfloat* positions, 
                              const GLfloat* colors,
                              const GLfloat* normals,
                              const GLfloat* uv,
                              const GLuint* indices,
                              int numVertices,
                              int numIndices,
                              GLenum drawMode)
{

}


}  // namespace gloo.