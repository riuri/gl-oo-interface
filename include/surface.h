/*
*   TODO: Add description.
*
*
*  // Vertices packed as (floats array):
*  // Stride = sizeof(float)*11
*  // Offset for color  = sizeof(float)*3
*  // Offset for normal = sizeof(float)*6
*  // Offset for texcoord = sizeof(float)*9 
*  //  -------------------------------------------------------------------------------------
*  // | x y z r g b nx ny nz tx ty | x y z r g b nx ny nz tx ty | x y z r g b nx ny nz tx ty| ...
*  //  -------------------------------------------------------------------------------------
*
*/
#pragma once

#include <iostream>
#include "openGLHeader.h"
#include "glutHeader.h"

#include "imageIO.h"
#include "openGLMatrix.h"
#include "basicPipelineProgram.h"

// Constants.
const GLint   kVertexSize = 11;      // Tells how many floats are stored for each vertex in VBO.
const GLfloat kSurfaceWidth = 10.0f;

enum DisplayMode { kPoints, kWireFrame, kSolid };

struct Material
{
  GLfloat Ka;  // Ambient component.
  GLfloat Kd;  // Diffuse component.
  GLfloat Ks;  // Specular component.
};

class Surface 
{
public:
  Surface();
  ~Surface();

  // Builds up the surface from a loaded image.
  void LoadHeightmap(ImageIO *image, ImageIO *texture);

  // Display.
  void Draw();

  // Mouse interaction method.
  void IntersectRay(const glm::vec3& C, const glm::vec3& r, int signal);
  
  inline void SetDisplayMode(DisplayMode displayMode) 
  { 
    this->displayMode = displayMode; 
  }

  inline void SetPipelineProgramParam(BasicPipelineProgram *pipelineProgram, GLuint programHandle) 
  { 
    this->programHandle = programHandle; 
    this->pipelineProgram = pipelineProgram;
  } 

private:
  void DeformLandscap(int xc, int yc, int signal);

  /////////////////////////////////////////////////////////////////////////////////////////////////
  // Creates VBO, VAO, EAB and TEX buffers.
  void CreateBufferObjects(ImageIO* texture);  

  // Buffers init/deinit methods.
  void AllocateGeometryArrays(int w, int h);
  void ClearGeometryArrays(void);

  // Vertex and Element arrays initialization.
  void InitVertexArray(ImageIO *image);
  void InitElementArrays();

  /////////////////////////////////////////////////////////////////////////////////////////////////

  void LoadPositions(ImageIO *image, unsigned bytesPerPixel);
  
  // Colors the terrain according to a convex combination of (r_min, g_min, b_min) and
  // (r_max, g_max, b_max). It uses the normalized height as parametric variable.
  void LoadColors(GLfloat r_min, GLfloat g_min, GLfloat b_min, 
                GLfloat r_max, GLfloat g_max, GLfloat b_max);

  // Colors the terrain copying from the input image.
  void LoadColors(ImageIO* image);
  void LoadNormals();
  void LoadTextureCoordinates();

  /////////////////////////////////////////////////////////////////////////////////////////////////

  // Following methods return the corresponding buffer size IN BYTES.
  inline size_t GetVerticesSize() const { return this->numVertices * kVertexSize * sizeof(GLfloat); }
  inline size_t GetElementPointsSize()    const { return this->numElementPoints    * sizeof(GLint); }
  inline size_t GetElementWireFrameSize() const { return this->numElementWireFrame * sizeof(GLint); }
  inline size_t GetElementSolidSize()     const { return this->numElementSolid     * sizeof(GLint); }

  // Takes images coordinates as input and returns the reference for the pixel value.
  inline GLfloat& PixelAt(int x, int y) const
  {
    return  this->vertices[kVertexSize * ((width * y) + x) + 1];  
  }
  
  // Returns the address of x coordinate from vertex at (x, y) (pixel coordinates). 
  inline GLfloat* VertexAt(int x, int y)
  {
    return &this->vertices[kVertexSize * ((width * y) + x)];
  }
  // Returns the address of Red color from vertex (x, y) (pixel coordinates).
  inline GLfloat* ColorAt(int x, int y)
  {
    return &this->vertices[kVertexSize * ((width * y) + x) + 3];
  }
  // Returns the address of normal vector array from vextex (x, y) (pixel coordinates).
  inline GLfloat* NormalAt(int x, int y)
  {
    return &this->vertices[kVertexSize * ((width * y) + x) + 6];
  }
  // Returns the address of Texture coordinates of vertex (x, y) (pixel coordinates).
  inline GLfloat* TexCoordAt(int x, int y)
  {
    return &this->vertices[kVertexSize * ((width * y) + x) + 9];
  }

  // Attributes ...................
  // Buffer and display data.
  BasicPipelineProgram *pipelineProgram;
  GLuint programHandle;

  DisplayMode displayMode;
  bool buffersInitialized;
 
  // Vertex Buffer Obj, Vertex Array Obj and texture.
  GLuint vbo, vao, tex;

  // Element array for different display modes.
  GLuint eab[3];

  OpenGLMatrix modelMatrix;
  GLfloat *vertices;        // VBO - position, color, normal and texture coordinates.
  GLint *indicesPoints;     // To render as points;
  GLint *indicesWireFrame;  // To render as wire frame;
  GLint *indicesSolid;      // To render as triangle strip (solid).
  Material material;

  // Geometry data.
  GLfloat tileSize;           // Tells vertical/horizontal distance between vertices.
  size_t width, height;
  size_t numVertices;
  size_t numElementPoints;
  size_t numElementWireFrame;
  size_t numElementSolid;
};




