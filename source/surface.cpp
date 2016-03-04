#include "surface.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor and destructor.

Surface::Surface() 
{
  this->programHandle = 0;
  this->pipelineProgram = nullptr;
  this->buffersInitialized = false;
  this->displayMode = kSolid;
  this->width  = 0;
  this->height = 0;

  this->vertices = nullptr;
  this->indicesPoints    = nullptr;
  this->indicesWireFrame = nullptr;
  this->indicesSolid     = nullptr;
  
  this->numVertices = 0;
  this->numElementPoints = 0;
  this->numElementWireFrame = 0;
  this->numElementSolid = 0;

  this->material.Ka = 0.1f;
  this->material.Kd = 0.8f;
  this->material.Ks = 0.1f;
}

Surface::~Surface()
{
  Surface::ClearGeometryArrays();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Draw and update methods.
void Surface::Draw()
{
  // Set Material.
  GLuint locKa = glGetUniformLocation(this->programHandle, "material.Ka");
  GLuint locKd = glGetUniformLocation(this->programHandle, "material.Kd");
  GLuint locKs = glGetUniformLocation(this->programHandle, "material.Ks");
  glUniform1f(locKa, this->material.Ka);
  glUniform1f(locKd, this->material.Kd);
  glUniform1f(locKs, this->material.Ks);

  // Position object.
  this->modelMatrix.SetMatrixMode(OpenGLMatrix::ModelView);
  this->modelMatrix.LoadIdentity();
  this->modelMatrix.Translate(-tileSize*this->width/2.0, -0.5f, -tileSize*this->height/2.0);
  this->pipelineProgram->SetModelMatrix(this->modelMatrix);

  // Draw according to the display mode.
  glBindVertexArray(this->vao);

  if (this->displayMode == kPoints)
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eab[0]);
    glDrawElements(
     GL_POINTS,             // mode.
     this->numVertices,     // number of vertices.
     GL_UNSIGNED_INT,       // type.
     (void*)0               // element array buffer offset.
    );
  }
  else if(this->displayMode == kWireFrame)
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eab[1]);
    glDrawElements(
     GL_LINE_STRIP,             // mode.
     this->numElementWireFrame, // number of vertices.
     GL_UNSIGNED_INT,           // type.
     (void*)0                   // element array buffer offset.
    );
  }
  else
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eab[2]);
    glDrawElements(
     GL_TRIANGLE_STRIP,         // mode.
     this->numElementSolid,     // number of vertices.
     GL_UNSIGNED_INT,           // type.
     (void*)0                   // element array buffer offset.
    );
  }
}

void Surface::DeformLandscap(int xc, int yc, int signal) 
{
  for (int y = 0; y < this->height; y++) 
  {
    for (int x = 0; x < this->width; x++)
    {
      float* vertex = this->VertexAt(x, height-y-1);
      float* color  = this->ColorAt(x, height-y-1);

      float dist = sqrt( pow((x-xc), 2.0) 
                       + pow((y-yc), 2.0)  
                       ); 
      float var = this->width/10.0f;
      float blah = signal * (1.0/(sqrt(2*M_PI)* var)) * exp(-pow(dist/var, 2.0));
      vertex[1] += blah;
      // color[0] += blah/4;
      // color[1] += blah/4;
      // color[2] += blah/4;
    }
  }

  this->LoadNormals();
  this->LoadColors(0.0f, 0.0f, 0.7f, 1.0f, 0.5f, 0.0f);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  glBufferData(GL_ARRAY_BUFFER, this->GetVerticesSize(), this->vertices, GL_DYNAMIC_DRAW); 
}

void Surface::IntersectRay(const glm::vec3& C, const glm::vec3& r, int signal)
{
  // Transform coordinates from view to model.
  glm::mat4 M_inv = glm::inverse(this->modelMatrix.GetGLMatrix());
  glm::vec4 v = M_inv * glm::vec4(r, 0.0);  // Orientation vector.
  glm::vec4 O = M_inv * glm::vec4(C, 1.0);  // Line origin.
  
  O /= O[3];
  v = glm::normalize(v);
  
  float t = -O[1]/v[1];
  glm::vec4 P = O + t*v;

  int x = static_cast<int>(P[0] / tileSize);
  int y = static_cast<int>(P[2] / tileSize);
  
  if (x >= 0 && x < this->width
   && y >= 0 && y < this->height)
  {
    //std::cout << "x=" << x << " y=" << y << "\n";
    this->DeformLandscap(x, y, signal);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Loading methods - image, texture.
void Surface::LoadHeightmap(ImageIO *image, ImageIO *texture)
{
  if (image != nullptr)
  {
    Surface::ClearGeometryArrays();
    Surface::AllocateGeometryArrays(image->getWidth(), image->getHeight());
    Surface::InitElementArrays();
    Surface::InitVertexArray(image);
    Surface::CreateBufferObjects(texture);
  } 
  else 
  {
    std::cerr << "ERROR Input image is NULL.\n";
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// OpenGL buffers methods.
void Surface::CreateBufferObjects(ImageIO* texture)
{
  if (this->programHandle == 0)
  {
    std::cerr << "ERROR Program Handle must be set before creating buffer objects.\n";
    return;
  }

  if (this->buffersInitialized)  // Delete previous buffers.
  {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteTextures(1, &this->tex);
    glDeleteBuffers(1, &this->vbo);
    glDeleteBuffers(3, this->eab);
  }

  // Generate Buffers.
  glGenVertexArrays(1, &this->vao);
  glGenBuffers(1, &this->vbo);
  glGenBuffers(3, this->eab);

  // Specify VAO.
  glBindVertexArray(vao);
  
  // Send data to GPU.
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  glBufferData(GL_ARRAY_BUFFER, this->GetVerticesSize(), this->vertices, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eab[0]);  // Visualize as points.
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
    this->GetElementPointsSize(), this->indicesPoints, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eab[1]);  // Visualize as wireframe.
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
    this->GetElementWireFrameSize(), this->indicesWireFrame, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eab[2]);  // Visualize as triangle strip mesh.
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
    this->GetElementSolidSize(), this->indicesSolid, GL_DYNAMIC_DRAW);

  // Specify how the arguments will be passed to shaders.
  GLuint locTexCoordAttrib = glGetAttribLocation(this->programHandle, "in_tex_coord");
  GLuint locPositionAttrib = glGetAttribLocation(this->programHandle, "in_position");
  GLuint locNormalAttrib   = glGetAttribLocation(this->programHandle, "in_normal");
  GLuint locColorAttrib    = glGetAttribLocation(this->programHandle, "in_color");
  GLuint locTexOn = glGetUniformLocation(this->programHandle, "tex_on");
  GLsizei stride = sizeof(GLfloat) * kVertexSize;

  glEnableVertexAttribArray(locPositionAttrib);
  glEnableVertexAttribArray(locColorAttrib);
  glEnableVertexAttribArray(locNormalAttrib);

  glVertexAttribPointer(locPositionAttrib, 3, GL_FLOAT, GL_FALSE, stride, 0);
  glVertexAttribPointer(locColorAttrib,    3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat) * 3));
  glVertexAttribPointer(locNormalAttrib,   3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat) * 6));

  glUniform1i(locTexOn, (texture != nullptr));

  // If any texture is loaded.
  if (texture != nullptr)  
  {
    glGenTextures(1, &this->tex);
    glBindTexture(GL_TEXTURE_2D, this->tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int bytesPerPixel = texture->getBytesPerPixel();
    GLint internalFormat[] = {0, GL_ALPHA, GL_RGB, GL_RGBA};

    // From https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexImage2D.xml
    glTexImage2D( GL_TEXTURE_2D,  // Target.
                  0,              // Detail level - original.
                  GL_RGB,  // How the colors are stored.
                  texture->getWidth(),  // Width.
                  texture->getHeight(), // Height.
                  0,                    // Border must be 0. 
                  GL_RGB,  // Format (?).  
                  GL_UNSIGNED_BYTE,               // Data type.
                  texture->getPixels()            
    );

    void* offset = (void*)(sizeof(GLfloat) * 9);
    glEnableVertexAttribArray(locTexCoordAttrib);
    glVertexAttribPointer(locTexCoordAttrib, 2, GL_FLOAT, GL_FALSE, stride, offset);
  }
}

void Surface::InitVertexArray(ImageIO *image)
{
  unsigned bytesPerPixel = image->getBytesPerPixel();

  this->LoadPositions(image, bytesPerPixel);
  this->LoadNormals();
  this->LoadTextureCoordinates();

  if (bytesPerPixel == 1)
    this->LoadColors(0.0f, 0.0f, 0.7f, 1.0f, 0.5f, 0.0f);
  else
    this->LoadColors(image);
}

#define INDEX(a, b) ((this->width * (b)) + a)

void Surface::InitElementArrays()
{
  // Points Element array - simple indices.
  for (int i = 0; i < this->numElementPoints; i++)
  {
    this->indicesPoints[i] = i;
  }

  // Wire frame Element array - indices are written in a zig-zag pattern,
  // first horizontally and then vertically. It uses GL_LINE_STRIP. 
  int index = 0;
  int x = 0, y = 0;
  int dx = 1, dy = -1;

  for (y = 0; y < this->height; y++)  // Horizontally.
  {
    x = ((dx == 1) ? 0 : this->width-1);
    for (int k = 0; k < this->width; k++, x += dx)
      this->indicesWireFrame[index++] = INDEX(x, y);
    dx *= -1;
  }

  // Start from the last point to allow continuity in GL_LINE_STRIP.
  x = ((dx == 1) ? 0 : this->width-1);
  y = this->height-1;  
  dy = -1;
  for (int i = 0; i < this->width; i++, x += dx)  // Vertically.
  {
    y = ((dy == 1) ? 0 : this->height-1);
    for (int j = 0; j < this->height; j++, y += dy)
      this->indicesWireFrame[index++] = INDEX(x, y);
    dy *= -1;
  }

  // Solid Element array (triangle strip).
  index = 0;
  for (int y = 0; y < this->height-1; y++)
  {
    // Zig-zag pattern: alternate between top and bottom.
    for (int x = 0; x < this->width; x++)
    {
      this->indicesSolid[index++] = INDEX(x, y);
      this->indicesSolid[index++] = INDEX(x, y+1);
    }

    // Triangle row transition: handle discontinuity.
    if (y < this->height-2)
    {
      // Repeat last vertex and the next row first vertex to generate 
      // two invalid triangles and get continuity in the mesh.
      this->indicesSolid[index++] = INDEX(this->width-1, y+1);
      this->indicesSolid[index++] = INDEX(0, y+1);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Geometry and property changing methods.

void Surface::LoadPositions(ImageIO* image, unsigned bytesPerPixel)
{
  if (this->vertices == nullptr)
    return;

  for (int y = 0; y < this->height; y++)
  {
    for (int x = 0; x < this->width; x++)
    {
      GLfloat* position = this->VertexAt(x, y);
      position[0] = tileSize*x;                 // x.
      position[2] = tileSize*(height - 1 - y);  // z.

      // Next section handles RGB and grayscale images.
      if (bytesPerPixel == 3)  // Colorful image.
      {
        // RGB to grayscale conversion.
        position[1] = 0.299*image->getPixel(x, y, 0)/255.0
                    + 0.587*image->getPixel(x, y, 1)/255.0
                    + 0.114*image->getPixel(x, y, 2)/255.0;
      }
      else  // Graycale image.
      {
        position[1] = image->getPixel(x, y, 0)/255.0;
      }

    }
  }
}

void Surface::LoadColors(GLfloat r_min, GLfloat g_min, GLfloat b_min,
                         GLfloat r_max, GLfloat g_max, GLfloat b_max)
{
  if (this->vertices == nullptr)
    return;

  for (int y = 0; y < this->height; y++)
  {
    for(int x = 0; x < this->width; x++)
    {
        GLfloat* color = this->ColorAt(x, y);   // Vertex color.
        GLfloat t = this->PixelAt(x, y);        // Vertex Height.
        t = std::min<float>(t, 1.0);
        t = std::max<float>(t, 0.0);
        t = sqrt(sqrt(t));

        color[0] = (1-t)*r_min + (t)*r_max;
        color[1] = (1-t)*g_min + (t)*g_max;
        color[2] = (1-t)*b_min + (t)*b_max;
    }
  }
}

void Surface::LoadColors(ImageIO* image)
{
  if (this->vertices == nullptr)
    return;

  for (int y = 0; y < this->height; y++)
  {
    for(int x = 0; x < this->width; x++)
    {
        GLfloat* color = this->ColorAt(x, y);   // Vertex color.
        color[0] = image->getPixel(x, y, 0)/255.0;
        color[1] = image->getPixel(x, y, 1)/255.0;
        color[2] = image->getPixel(x, y, 2)/255.0;
    }
  } 
}

void Surface::LoadNormals()
{
  if (this->vertices == nullptr)
    return;

  for (int y = 0; y < this->height-1; y++)
  {
    for(int x = 0; x < this->width-1; x++)
    {
      // Simple gradient calculation.
      GLfloat* normal = this->NormalAt(x, y);
      GLfloat& right  = this->PixelAt(x+1, y);
      GLfloat& bottom = this->PixelAt(x, y+1);
      GLfloat& pixel  = this->PixelAt(x, y);

      glm::vec3 dx(1.0f, (right  - pixel)/kVertexSize*255, 0.0f);  // Gradient along each row.
      glm::vec3 dz(0.0f, (bottom - pixel)/kVertexSize*255, 1.0f);  // Gradient along each column.
      glm::vec3 n = glm::cross(dz, dx);
      //n = glm::normalize(n);

      memcpy(normal, &n[0], sizeof(float) * 3);
    }
  }

  int y = this->height-1;
  for (int x = 0; x < this->width-1; x++) 
  {
    GLfloat* normal1 = this->NormalAt(x, y-1);
    GLfloat* normal2 = this->NormalAt(x, y);
    memcpy(normal2, normal1, sizeof(float) * 3);
  }

  int x = this->width-1;
  for (int y = 0; y < this->height-1; y++) 
  {
    GLfloat* normal1 = this->NormalAt(x-1, y);
    GLfloat* normal2 = this->NormalAt(x, y);
    memcpy(normal2, normal1, sizeof(float) * 3);
  }
}

void Surface::LoadTextureCoordinates()
{
  if (this->vertices == nullptr)
    return;

  for (int y = 0; y < this->height; y++)
  {
    for(int x = 0; x < this->width; x++)
    {
      GLfloat* texCoord = this->TexCoordAt(x, y);
      texCoord[0] = (GLfloat)x/(this->width  - 1)*2;
      texCoord[1] = (GLfloat)y/(this->height - 1)*2;
    }
  } 
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Surface::AllocateGeometryArrays(int w, int h)
{
  this->width  = w;
  this->height = h;
  this->tileSize = kSurfaceWidth/std::max(w, h);  

  // Element arrays setup. 
  this->numVertices      = w * h;
  this->numElementPoints = w * h;
  this->numElementWireFrame = 2 * w * h;
  this->numElementSolid    = 2*(h-2)*w + 2*w + 2*(h-2);

  // NOTE: Two additional indices must be added for each row transition to create
  // degenerate triangles and to allow continuity in the triangle strip.

  this->vertices = new GLfloat [kVertexSize * this->numVertices];
  this->indicesPoints    = new GLint [this->numVertices];
  this->indicesWireFrame = new GLint [this->numElementWireFrame];
  this->indicesSolid     = new GLint [this->numElementSolid];
}

void Surface::ClearGeometryArrays(void)
{
  delete [] this->vertices;
  delete [] this->indicesPoints;
  delete [] this->indicesWireFrame;
  delete [] this->indicesSolid;
}
