#include "mesh.h"

#include <fstream>
#include <sstream>
#include <cstdio>

namespace gloo
{

void Mesh::Render() const
{
  if (IsInitialized()) 
  {
    glBindVertexArray(mVao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEab);

    glDrawElements(
     mDrawMode,         // mode.
     mNumIndices,       // number of vertices.
     GL_UNSIGNED_INT,   // type.
     (void*)0           // element array buffer offset.
    );
  }
}

bool Mesh::Load(const GLfloat* positions,
                const GLfloat* colors   , 
                const GLfloat* normals  ,
                const GLfloat* texCoords,
                const GLuint* indices  ,
                int numVertices, int numIndices,
                GLenum drawMode, StorageType storageType
              ) 
{ 
  if (!positions || numVertices <= 0)
  {
    return false;
  }

  mDrawMode = drawMode;
  mStorageType = storageType;
  mNumVertices = numVertices;
  mNumIndices  = (numIndices <= 0 || indices == nullptr) ? numVertices : numIndices;

  mHasColors   = (colors    != nullptr);
  mHasNormals  = (normals   != nullptr);
  mHasTexCoord = (texCoords != nullptr);
  mVertexSize  = 3 + 3*mHasColors + 3*mHasNormals + 2*mHasTexCoord;

  mVertices = new GLfloat [mVertexSize * mNumVertices];
  mIndices  = new GLuint  [mNumIndices];

  if (mStorageType == kTightlyPacked)
  {
    // Initialize vertices buffer array. 
    for (int i = 0; i < mNumVertices; i++)
    {
      float* position = PositionAt(i);
      float* texCoord = TexCoordAt(i);
      float* normal   = NormalAt(i); 
      float* color    = ColorAt(i);

      memcpy(position, positions + 3*i, sizeof(GLfloat)*3);
      if (HasColors())
      {
        memcpy(color, colors + 3*i, sizeof(GLfloat)*3);
      }
      if (HasNormals())
      {
        memcpy(normal, normals + 3*i, sizeof(GLfloat)*3);
      }
      if (HasTexCoord())
      {
        memcpy(texCoord, texCoords + 2*i, sizeof(GLfloat)*2);
      }
    }

    // Initialize element array (indices array).
    if (indices)  // Element array provided.
    {
      memcpy(mIndices, indices, sizeof(GLuint)*mNumIndices);
    }
    else  // Element array wasn't provided -- build it up.
    {
      for (int i = 0; i < mNumIndices; i++)
      {
        mIndices[i] = i;
      }
    }
  }
  else
  {

  }

  mInitialized = true;
  Mesh::UploadGLBuffers();
  return true;
}

bool Mesh::Load(const GLfloat* vertices, const GLuint* indices, 
                int numVertices,         int numIndices, 
                bool hasColors,          bool hasNormals, 
                bool hasTexCoord,        GLenum drawMode
              )
{
  if (!vertices || numVertices <= 0)
  {
    return false;
  }

  mDrawMode = drawMode;
  mNumVertices = numVertices;
  mNumIndices  = (numIndices <= 0 || indices == nullptr) ? numVertices : numIndices;

  mHasColors   = hasColors;
  mHasNormals  = hasNormals;
  mHasTexCoord = hasTexCoord;
  mVertexSize  = 3 + 3*hasColors + 3*hasNormals + 2*hasTexCoord;

  mVertices = new GLfloat [mVertexSize * mNumVertices];
  mIndices  = new GLuint  [mNumIndices];

  // Initialize vertices buffer array. 
  memcpy(mVertices, vertices, sizeof(GLfloat)*mVertexSize*mNumVertices);

  // Initialize element array (indices array).
  if (indices)  // Element array provided.
  {
    memcpy(mIndices, indices, sizeof(GLuint)*mNumIndices);
  }
  else  // Element array wasn't provided -- build it up.
  {
    for (int i = 0; i < mNumIndices; i++)
    {
      mIndices[i] = i;
    }
  }

  mInitialized = true;
  Mesh::UploadGLBuffers();
  return true;
}

void Mesh::UploadGLBuffers()
{
  if (mProgramHandle == 0)
  {
    std::cerr << "ERROR Program Handle must be set before creating buffer objects.\n";
    return;
  }

  if (!mInitialized)
  {
    std::cerr << "ERROR The mesh must be initialized before creating buffer objects.\n";
    return;
  }

  // Specify how the arguments will be passed to shaders.
  GLuint locTexCoordAttrib = glGetAttribLocation(mProgramHandle, "in_tex_coord");
  GLuint locPositionAttrib = glGetAttribLocation(mProgramHandle, "in_position");
  GLuint locNormalAttrib   = glGetAttribLocation(mProgramHandle, "in_normal");
  GLuint locColorAttrib    = glGetAttribLocation(mProgramHandle, "in_color");

  // Generate Buffers.
  glGenVertexArrays(1, &mVao);
  glGenBuffers(1, &mVbo);
  glGenBuffers(1, &mEab);

  // Specify VAO.
  glBindVertexArray(mVao);
  
  // Upload indices to GPU.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEab);  
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumIndices * sizeof(GLuint), mIndices, GL_STATIC_DRAW);

  // Enable/Disable each vertex attribute.
  glEnableVertexAttribArray(locPositionAttrib);
  if (HasColors())
  {
    glEnableVertexAttribArray(locColorAttrib);
  }
  else
  {
    glDisableVertexAttribArray(locColorAttrib);
  }
  
  if (HasNormals())
  {
    glEnableVertexAttribArray(locNormalAttrib);
  }
  else
  {
    glDisableVertexAttribArray(locNormalAttrib);
  }

  if (HasTexCoord())
  { 
    glEnableVertexAttribArray(locTexCoordAttrib);
  }
  else
  {
    glDisableVertexAttribArray(locTexCoordAttrib);
  }

  // Upload vertices to GPU.
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glBufferData(GL_ARRAY_BUFFER, mVertexSize * mNumVertices * sizeof(GLfloat), mVertices, GL_STATIC_DRAW);

  if (mStorageType == kTightlyPacked)
  {
    GLfloat stride = sizeof(GLfloat) * mVertexSize;
    glVertexAttribPointer(locPositionAttrib, 3, GL_FLOAT, GL_FALSE, stride, 0);
    
    glVertexAttribPointer(locColorAttrib, 3, GL_FLOAT, GL_FALSE, stride, 
      (void*)(sizeof(GLfloat) * 3));

    glVertexAttribPointer(locNormalAttrib, 3, GL_FLOAT, GL_FALSE, stride, 
      (void*)(sizeof(GLfloat) * (3 + 3*mHasColors)));

    glVertexAttribPointer(locTexCoordAttrib, 2, GL_FLOAT, GL_FALSE, stride, 
      (void*)(sizeof(GLfloat) * (3 + 3*mHasColors + 3*mHasNormals)) );
  }
  else   // Sub buffered storage type.
  {
    glVertexAttribPointer(locPositionAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glVertexAttribPointer(locColorAttrib,    3, GL_FLOAT, GL_FALSE, 0, 
      (void*)(sizeof(GLfloat) * 3*mNumVertices));
    
    glVertexAttribPointer(locNormalAttrib,   3, GL_FLOAT, GL_FALSE, 0, 
      (void*)(sizeof(GLfloat) * (3 + 3*mHasColors)*mNumVertices));

    glVertexAttribPointer(locTexCoordAttrib, 2, GL_FLOAT, GL_FALSE, 0,   
      (void*)(sizeof(GLfloat) * (3 + 3*mHasColors + 3*mHasNormals)*mNumVertices));
  }
}

// ============================================================================================= //

void Mesh::Preallocate(int numVertices, int numIndices, bool hasColors, 
                       bool hasNormals, bool hasTexCoord)
{
  mNumVertices = numVertices;
  mNumIndices  = numIndices;

  mHasColors   = hasColors;
  mHasNormals  = hasNormals;
  mHasTexCoord = hasTexCoord;

  mVertexSize = (3 + 3*hasColors + 3*hasNormals + 2*hasTexCoord);

  mVertices = new GLfloat [mVertexSize * mNumVertices];
  mIndices  = new GLuint  [mNumIndices];

  mInitialized = true;
}

void Mesh::LoadFromObj(const std::string& objFilename)
{
    /* Courtesy from https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ */
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoord;
    std::vector<GLuint> elements;

    float  x, y, z;
    GLuint a, b, c;

    std::ifstream in(objFilename, std::ios::in);
    if (!in)
    {
        std::cerr << "ERROR Couldn't open " << objFilename << std::endl;
    }

    std::string line;
    while (getline(in, line))
    {
      if (line.substr(0,2) == "v ")
      {
        std::istringstream s(line.substr(2));
        s >> x; s >> y; s >> z;
        positions.emplace_back(x, y, z);
      }
      else if (line.substr(0,3) == "vn ")
      {
        std::istringstream s(line.substr(3));
        s >> x; s >> y; s >> z;
        normals.emplace_back(x, y, z);
      }
      else if (line.substr(0,2) == "f ")
      {
        std::istringstream s(line.substr(2));
        
        s >> a; s >> b; s >> c;
        a--; b--; c--;
        elements.push_back(a); 
        elements.push_back(b); 
        elements.push_back(c);
      }
      else if (line.substr(0,2) == "t ")
      {
        std::istringstream s(line.substr(2));
        s >> x; s >> y;
        texCoord.emplace_back(x, y);
      }
      else if (line[0] == '#')
      {
          /* ignoring this line */
      }
      else
      {
          /* ignoring this line */
      }
    }

    if (normals.size() == 0)  // Normals weren't provided in the obj file.
    {
      normals.resize(positions.size(), glm::vec3(0.0, 0.0, 0.0));
      for (int i = 0; i < elements.size(); i+=3)
      {
          a = elements[i];
          b = elements[i+1];
          c = elements[i+2];
          glm::vec3 normal = glm::normalize(glm::cross(
          glm::vec3(positions[b]) - glm::vec3(positions[a]),
          glm::vec3(positions[c]) - glm::vec3(positions[a])));
          normals[a] = normals[b] = normals[c] = normal;
      }
    }

    mHasColors  = false;
    mHasNormals = true;
    mHasTexCoord = (texCoord.size() > 0 );
    mVertexSize = 3 + 3*mHasColors + 3*mHasNormals + 2*mHasTexCoord;
    mDrawMode = GL_TRIANGLES;

    mNumVertices = positions.size();
    mNumIndices  = elements.size();

    mVertices = new GLfloat [mVertexSize * mNumVertices];
    mIndices  = new GLuint  [mNumIndices];

    for (int i = 0; i < mNumVertices; i++)
    {
      float* position = PositionAt(i);
      float* normal   = NormalAt(i);
      //float* color    = ColorAt(i);

      position[0] = positions[i][0];
      position[1] = positions[i][1];
      position[2] = positions[i][2];

      normal[0] = normals[i][0];
      normal[1] = normals[i][1];
      normal[2] = normals[i][2];
    }

    for (int i = 0; i < mNumIndices; i++)
    {
      mIndices[i] = elements[i];
    }

    mInitialized = true;
    Mesh::UploadGLBuffers();
}

void Mesh::LoadFromPositions(const std::vector<glm::vec3>& positions, GLenum drawMode,
                             float r, float g, float b)
{
  int n = positions.size();
  mNumVertices = n;
  mNumIndices  = n;
  mDrawMode = drawMode;

  mVertexSize = 6;
  mHasColors  = true;
  mHasNormals = false;
  mHasTexCoord = false;

  mVertices = new GLfloat [mVertexSize * mNumVertices];
  mIndices  = new GLuint  [mNumIndices];

  for (int i = 0; i < n; i++)
  {
    float* position = PositionAt(i);
    float* color    = ColorAt(i);

    position[0] = positions[i][0];
    position[1] = positions[i][1];
    position[2] = positions[i][2];

    color[0] = r;
    color[1] = g;
    color[2] = b;

    mIndices[i] = i;
  }

  mInitialized = true;
  Mesh::UploadGLBuffers();
}

void Mesh::UpdateFromPositions(const std::vector<glm::vec3>& positions)
{
  int n = positions.size();

  for (int i = 0; i < n; i++)
  {
    float* position = PositionAt(i);

    position[0] = positions[i][0];
    position[1] = positions[i][1];
    position[2] = positions[i][2];
  }

  Mesh::Update();
}

void Mesh::Update()
{
  // Update vertices to GPU.
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glBufferData(GL_ARRAY_BUFFER, mVertexSize * mNumVertices * sizeof(GLfloat), mVertices, GL_STATIC_DRAW);
}

// ============================================================================================= //

Mesh::~Mesh()
{
  if (mInitialized)
  {
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers(1, &mVbo);
    glDeleteBuffers(1, &mEab);
  }

  delete [] mVertices;
  delete [] mIndices;
}

} // namespace gloo.