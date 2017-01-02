#include "useful_meshes.h"

namespace gloo
{

// ============================================================================================= //

AxisMesh::AxisMesh(GLint positionAttribLoc, GLint colorAttribLoc)
{
  const int numVertices = 6;
  const int numElements = 6;
  const GLenum drawMode = GL_LINES;

  // Initialize vertices.
  GLfloat positions[] = {0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
                         0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f};
  GLfloat colors [] =   {1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
                         0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
                         0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f};
  const GLuint* indices = nullptr;

  // Allocate mesh.
  mMeshGroup = new MeshGroup<Batch>(numVertices, numElements, drawMode);

  // Specify its attributes.
  mMeshGroup->SetVertexAttribList({3, 3});

  // Add rendering pass.
  mMeshGroup->AddRenderingPass({{positionAttribLoc, true}, {colorAttribLoc, true}});

  // Load data.
  mMeshGroup->Load({positions, colors}, indices);
}

AxisMesh::~AxisMesh()
{
  delete mMeshGroup;
}

void AxisMesh::Update(GLfloat x, GLfloat y, GLfloat z)
{
  GLfloat positions[] = {0.0f, 0.0f, 0.0f,     x, 0.0f, 0.0f,
                         0.0f, 0.0f, 0.0f,  0.0f,    y, 0.0f,
                         0.0f, 0.0f, 0.0f,  0.0f, 0.0f,    z};

  mMeshGroup->Update({positions, nullptr});
}

void AxisMesh::Render() const
{
  mMeshGroup->Render();
}

// ============================================================================================= //

BoundingBoxMesh::BoundingBoxMesh(GLint positionAttribLoc, GLint colorAttribLoc, 
                                 const glm::vec3 & rgb)
{
  const int numVertices = 8;
  const int numElements = 12*2;
  const GLenum drawMode = GL_LINES;

  const GLfloat r = rgb[0];
  const GLfloat g = rgb[1];
  const GLfloat b = rgb[2];

  // Initialize vertices.
  GLfloat positions[] = {-0.5f, +0.5f, +0.5f,  +0.5f, +0.5f, +0.5f,
                         -0.5f, +0.5f, -0.5f,  +0.5f, +0.5f, -0.5f,
                         -0.5f, -0.5f, +0.5f,  +0.5f, -0.5f, +0.5f,
                         -0.5f, -0.5f, -0.5f,  +0.5f, -0.5f, -0.5f };

  GLfloat colors [] =   {r, g, b, r, g, b,
                         r, g, b, r, g, b,
                         r, g, b, r, g, b,
                         r, g, b, r, g, b};
  GLuint indices[] = { 0,1, 2,3, 4,5, 6,7,    // x
                       0,2, 1,3, 4,6, 5,7,    // z
                       0,4, 1,5, 2,6, 3,7 };  // z

  // Allocate mesh.
  mMeshGroup = new MeshGroup<Batch>(numVertices, numElements, drawMode);

  // Specify its attributes.
  mMeshGroup->SetVertexAttribList({3, 3});

  // Add rendering pass.
  mMeshGroup->AddRenderingPass({{positionAttribLoc, true}, {colorAttribLoc, true}});

  // Load data.
  mMeshGroup->Load({positions, colors}, indices);
}

BoundingBoxMesh::~BoundingBoxMesh()
{
  delete mMeshGroup;
}

void BoundingBoxMesh::Update(GLfloat xmin, GLfloat xmax, GLfloat ymin, GLfloat ymax, GLfloat zmin, GLfloat zmax)
{
  GLfloat positions[] = {xmin, ymax, zmax,  xmax, ymax, zmax,
                         xmin, ymax, zmin,  xmax, ymax, zmin,
                         xmin, ymin, zmax,  xmax, ymin, zmax,
                         xmin, ymin, zmin,  xmax, ymin, zmin };

  mMeshGroup->Update({positions, nullptr});
}

void BoundingBoxMesh::Render() const
{
  mMeshGroup->Render();
}

// ============================================================================================= //

GridMesh::GridMesh(GLint positionAttribLoc, GLint colorAttribLoc, int width, int height, 
                   GLfloat tileSize, const glm::vec3 & rgb)
{
  const int w = std::max(width,  2);
  const int h = std::max(height, 2);

  const int numVertices = w * h;
  const int numElements = 2 * w * h;
  const GLenum drawMode = GL_LINE_STRIP;

  std::vector<GLfloat> vertices;
  std::vector<GLuint> indices;

  const GLfloat r = rgb[0];
  const GLfloat g = rgb[1];
  const GLfloat b = rgb[2];

  vertices.reserve(numVertices * 6);

  float xc = (w-1)*tileSize/2.0f;
  float yc = 0.0f;
  float zc = (h-1)*tileSize/2.0f;

  // Initialize vertices.
  for (int z = 0; z < h; z++)
  {
    for (int x = 0; x < w; x++)
    {
      // Positions x, y, z.
      vertices.push_back(x*tileSize - xc);
      vertices.push_back(0.0f);
      vertices.push_back(z*tileSize - zc);

      // Colors.
      vertices.push_back(r);
      vertices.push_back(g);
      vertices.push_back(b);
    }
  }

  indices.reserve(numElements);

  // Wire frame Element array - indices are written in a zig-zag pattern,
  // first horizontally and then vertically. It uses GL_LINE_STRIP. 
  int index = 0;
  int x = 0, y = 0;
  int dx = 1, dy = -1;

  for (y = 0; y < h; y++)  // Horizontally.
  {
    x = ((dx == 1) ? 0 : w-1);
    for (int k = 0; k < w; k++, x += dx)
      indices.push_back(w*x + y);
    dx *= -1;
  }

  // Start from the last point to allow continuity in GL_LINE_STRIP.
  x = ((dx == 1) ? 0 : w-1);
  y = h-1;  
  dy = -1;
  for (int i = 0; i < w; i++, x += dx)  // Vertically.
  {
    y = ((dy == 1) ? 0 : h-1);
    for (int j = 0; j < h; j++, y += dy)
      indices.push_back(w*x + y);
    dy *= -1;
  }

  // Allocate mesh.
  mMeshGroup = new MeshGroup<Interleave>(numVertices, numElements, drawMode);

  // Specify its attributes.
  mMeshGroup->SetVertexAttribList({3, 3});

  // Add rendering pass.
  mMeshGroup->AddRenderingPass({{positionAttribLoc, true}, {colorAttribLoc, true}});

  // Load data.
  // mMeshGroup->Load({positions.data(), colors.data()}, indices.data());
  mMeshGroup->Load(vertices.data(), indices.data());
}

GridMesh::~GridMesh()
{
  delete mMeshGroup;
}

void GridMesh::Render() const
{
  mMeshGroup->Render();
}

// ============================================================================================= //

Polygon::Polygon(GLint positionAttribLoc, GLint colorAttribLoc, float sideLength, int numSides,
                 const glm::vec3 & rgb, float xc, float yc)
{
  const int N = numSides;
  const float L = sideLength;

  const int numVertices = N+1;  // Center + N vertices.
  const int numElements = 3*N;  // Specify N triangles.
  const GLenum drawMode = GL_TRIANGLES;

  std::vector<GLfloat> positions;
  std::vector<GLfloat> colors;
  std::vector<GLuint> indices;

  positions.reserve(numVertices * 3);
  colors.reserve(numVertices * 3);
  indices.reserve(numElements);

  const GLfloat r = rgb[0];
  const GLfloat g = rgb[1];
  const GLfloat b = rgb[2];

  // Add center:
  positions.push_back(xc);
  positions.push_back(yc);
  positions.push_back(0.0f);

  colors.push_back(r);
  colors.push_back(g);
  colors.push_back(b);

  const float radius = (L/2.0f)/std::sin((M_PI)/N);

  // Add all vertices:
  for (int i = 0; i < N; i++) {
    float theta = static_cast<float>(i)/(N) * M_PI * 2.0f;

    positions.push_back(xc + radius * std::cos(theta));
    positions.push_back(yc + radius * std::sin(theta));
    positions.push_back(0.0f);

    colors.push_back(r*0.5f);
    colors.push_back(g*0.5f);
    colors.push_back(b*0.5f);

    indices.push_back(0);
    indices.push_back(i+1);
    if (i+2 > N) {
      indices.push_back(1);
    } else {
      indices.push_back(i+2);
    }
  }

  // Allocate mesh.
  mMeshGroup = new MeshGroup<Batch>(numVertices, numElements, drawMode);

  // Specify its attributes.
  mMeshGroup->SetVertexAttribList({3, 3});

  // Add rendering pass.
  mMeshGroup->AddRenderingPass({{positionAttribLoc, true}, {colorAttribLoc, true}});

  // Load data.
  mMeshGroup->Load({positions.data(), colors.data()}, indices.data());
}

Polygon::~Polygon()
{
  delete mMeshGroup;
}

void Polygon::Update()
{

}

void Polygon::Render() const
{
  mMeshGroup->Render();
}

// ============================================================================================= //

WireframeSphere::WireframeSphere(GLint positionAttribLoc, GLint colorAttribLoc, 
                                 const glm::vec3 & rgb, int detail) 
{
  int w = detail+1;
  int h = detail+1;

  const int numVertices = (w * h);
  const int numElements = (2 * w * h);
  const GLenum drawMode = GL_LINE_STRIP;

  std::vector<GLfloat> positions;
  std::vector<GLfloat> colors;
  std::vector<GLuint> indices;

  positions.reserve(numVertices * 3);
  colors.reserve(numVertices * 3);
  indices.reserve(numElements);

  const GLfloat r = rgb[0];
  const GLfloat g = rgb[1];
  const GLfloat b = rgb[2];

  // Initialize vertices.
  for (int v = 0; v < h; v++)
  {
    for (int u = 0; u < w; u++)
    {
      GLfloat theta_u = (2*M_PI * u) / (w-1);
      GLfloat theta_v =   (M_PI * v) / (h-1);
      GLfloat position[3];

      position[0] = cos(theta_u) * sin(theta_v);
      position[2] = sin(theta_u) * sin(theta_v);
      position[1] = cos(theta_v);

      // Vertex coordinates.
      positions.push_back(position[0]);
      positions.push_back(position[1]);
      positions.push_back(position[2]);

      // Vertex colors.
      colors.push_back(r);
      colors.push_back(g);
      colors.push_back(b);
    }
  }
  
  // Wireframe Element array - indices are written in a zig-zag pattern,
  // first horizontally and then vertically. It uses GL_LINE_STRIP. 
  int index = 0;
  int x = 0, y = 0;
  int dx = 1, dy = -1;

  for (y = 0; y < h; y++)  // Horizontally.
  {
    x = ((dx == 1) ? 0 : w-1);
    for (int k = 0; k < w; k++, x += dx)
      indices.push_back(w*y + x);  // INDEX(x, y).
    dx *= -1;
  }

  // Start from the last point to allow continuity in GL_LINE_STRIP.
  x = ((dx == 1) ? 0 : w-1);
  y = h-1;  
  dy = -1;
  for (int i = 0; i < w; i++, x += dx)  // Vertically.
  {
    y = ((dy == 1) ? 0 : h-1);
    for (int j = 0; j < h; j++, y += dy)
      indices.push_back(w*y + x);  // INDEX(x, y).
    dy *= -1;
  }

  // Allocate mesh.
  mMeshGroup = new MeshGroup<Batch>(numVertices, numElements, drawMode);

  // Specify its attributes.
  mMeshGroup->SetVertexAttribList({3, 3});

  // Add rendering pass.
  mMeshGroup->AddRenderingPass({{positionAttribLoc, true}, {colorAttribLoc, true}});

  // Load data.
  mMeshGroup->Load({positions.data(), colors.data()}, indices.data());
}

WireframeSphere::~WireframeSphere() 
{
  delete mMeshGroup;
}

void WireframeSphere::Update() 
{
  // Do nothing.
}

void WireframeSphere::Render() const
{
  mMeshGroup->Render();
}

// ============================================================================================= //

TexturedSphere::TexturedSphere(GLint positionAttribLoc, GLint normalAttribLoc, GLint uvAttribLoc,
  const Material & material)
{
  mMaterial = material;

  int w = 65;
  int h = 65;

  const int numVertices = (w * h);
  const int numElements = 2*(h-2)*w + 2*w + 2*(h-2);
  const GLenum drawMode = GL_TRIANGLE_STRIP;

  std::vector<GLfloat> positions;
  std::vector<GLfloat> normals;
  std::vector<GLfloat> uvs;
  std::vector<GLuint> indices;

  positions.reserve(numVertices * 3);
  normals.reserve(numVertices * 3);
  uvs.reserve(numVertices * 2);
  indices.reserve(numElements);

  // Initialize vertices.
  for (int v = 0; v < h; v++)
  {
    for (int u = 0; u < w; u++)
    {
      GLfloat theta_u = (2*M_PI * u) / (w-1);
      GLfloat theta_v =   (M_PI * v) / (h-1);
      GLfloat position[3];

      position[0] = cos(theta_u) * sin(theta_v);
      position[2] = sin(theta_u) * sin(theta_v);
      position[1] = cos(theta_v);

      // Vertex coordinates.
      positions.push_back(position[0]);
      positions.push_back(position[1]);
      positions.push_back(position[2]);

      // Vertex normals.
      normals.push_back(2*position[0]);
      normals.push_back(2*position[1]);
      normals.push_back(2*position[2]);

      // Vertex uvs.
      uvs.push_back(static_cast<float>(u)/(w-1));
      uvs.push_back(1 - static_cast<float>(v)/(h-1));
    }
  }

  for (int v = 0; v < h-1; v++)
  {
    // Zig-zag pattern: alternate between top and bottom.
    for (int u = 0; u < w; u++)
    {
      indices.push_back((v+0)*w + u);
      indices.push_back((v+1)*w + u);
    }

    // Triangle row transition: handle discontinuity.
    if (v < h-2)
    {
      // Repeat last vertex and the next row first vertex to generate 
      // two invalid triangles and get continuity in the mesh.
      indices.push_back((v+1)*w + (w-1)); //INDEX(this->width-1, y+1);
      indices.push_back((v+1)*w + 0);     //INDEX(0, y+1);
    }
  }

  // Allocate mesh.
  mMeshGroup = new MeshGroup<Batch>(numVertices, numElements, drawMode);

  // Specify its attributes.
  mMeshGroup->SetVertexAttribList({3, 3, 2});

  // Add rendering pass.
  mMeshGroup->AddRenderingPass({{positionAttribLoc, true},
                                {normalAttribLoc, true},
                                {uvAttribLoc, true}});

  // Load data.
  mMeshGroup->Load({positions.data(), normals.data(), uvs.data()}, indices.data());
}

TexturedSphere::~TexturedSphere() 
{
  delete mMeshGroup;
}

void TexturedSphere::Update() 
{
  // Do nothing.
}

void TexturedSphere::Render() const
{
  mMeshGroup->Render(0);
}

}  // namespace gloo.