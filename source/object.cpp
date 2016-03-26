#include "object.h"

#define INDEX(a, b) (((w) * (b)) + a)

namespace gloo
{

namespace obj
{

// ================= Renderer ===================== //
void Object::Render() const
{
  glDisable(GL_TEXTURE_2D);
  GLuint lightOnLoc = glGetUniformLocation(mProgramHandle, "light_on");
  GLuint matLoc = glGetUniformLocation(mProgramHandle, "material_on");
  GLuint locKa  = glGetUniformLocation(mProgramHandle, "material.Ka");
  GLuint locKd  = glGetUniformLocation(mProgramHandle, "material.Kd");
  GLuint locKs  = glGetUniformLocation(mProgramHandle, "material.Ks");
  GLuint texLoc = glGetUniformLocation(mProgramHandle, "tex_on");

  glUniform1i(lightOnLoc, mUsingLighting);
  glUniform1i(matLoc, 1);
  glUniform3f(locKa, 0.1f, 0.1f, 0.1f);
  glUniform3f(locKd, 0.5f, 0.0f, 0.0f);
  glUniform3f(locKs, 0.01f, 0.01f, 0.01f);
  glUniform1i(texLoc, 0);


  mModelMatrix.LoadIdentity();
  mModelMatrix.Translate(mPos[0], mPos[1], mPos[2]);
  mModelMatrix.Rotate(mRot[2], 0, 0, 1);
  mModelMatrix.Rotate(mRot[0], 1, 0, 0);
  mModelMatrix.Rotate(mRot[1], 0, 1, 0);
  mModelMatrix.Scale(mScale[0], mScale[1], mScale[2]);

  mPipelineProgram->SetModelMatrix(mModelMatrix);

  for (auto& group : mGroups)
  {
    group.mesh->Render();
  }
}

// ================= .obj Loader ================== //


void Object::BuildUpGroup(std::vector<GLfloat>& groupPositions, 
                          std::vector<GLfloat>& groupTexCoords, 
                          std::vector<GLfloat>& groupNormals,
                          char* name)
{
  // printf("Storing group %s...\n", name);
  // Create new group - mesh, material index and name.
  Mesh* mesh = new Mesh(mProgramHandle);
  int materialIndex = 0;

  mesh->Load(  &groupPositions[0], // Positions
               nullptr,            // Colors
               (groupNormals.size() != 0) ? &groupNormals[0] : nullptr,     // Normals
               (groupTexCoords.size() != 0) ? &groupTexCoords[0] : nullptr, // Texture coords.
               nullptr,                                                     // Indices.
               groupPositions.size()/3, 0, GL_TRIANGLES, Mesh::kSubBuffered
            );

  mGroups.emplace_back(mesh, materialIndex, name);
  name[0] = '-';
  name[1] = '\0';
}

bool Object::Load(const std::string& objFilepath)
{
  FILE* inFile = fopen(objFilepath.c_str(), "r");

  // Successfully opened the file.
  if (inFile != nullptr)
  {
    char line[256];  // Line buffer.
    char token[32];  // Token buffer.
    char name[100];  // Group name buffer.
    const int kNumIndices = 100;
    std::vector<GLfloat> positions;  // List of all positions (x, y, z).
    std::vector<GLfloat> texCoords;  // List of all texture (u, v).
    std::vector<GLfloat> normals;    // List of all normals (nx, ny, nz).

    std::vector<GLfloat> groupPositions;  // List of current group positions.
    std::vector<GLfloat> groupTexCoords;  // List of current group tex coords.
    std::vector<GLfloat> groupNormals;    // List of current group normals.

    bool buildingGroup = false;

    // Read until get EOF.
    while (fgets(line, sizeof(line), inFile) != nullptr)
    {
      // Exclude repeated spaces.
      int i = 0;
      while (line[i] != '\0')
      {
        if (line[i] == ' ' && line[i+1] == ' ')
        {
          for (int j = i; line[j] != '\0'; j++)
          {
            line[j] = line[j+1];
          }
        }       
        i++; 
      }

      // Read first token.
      sscanf(line, "%s", token);
      //printf("<%s> %s", token, line);

      if (strcmp(token, "g") == 0)  // New group.
      {
        if (buildingGroup)  // Finish storing previous group.
        {
          Object::BuildUpGroup(groupPositions, groupTexCoords, groupNormals, name);
          groupPositions = std::vector<GLfloat>();
          groupTexCoords = std::vector<GLfloat>();
          groupNormals   = std::vector<GLfloat>();
        }
        else
        {
          buildingGroup = true;
        }

        sscanf(line, "%s %s", token, name);
        // printf("New group: %s\n", name);
      }
      else if (strcmp(token, "v") == 0)   // New vertex.
      {
        float x, y, z;
        sscanf(line, "%s %f %f %f", token, &x, &y, &z);
        positions.push_back(x);
        positions.push_back(y);
        positions.push_back(z);
      }
      else if (strcmp(token, "vt") == 0)  // New texture coordinate.
      {
        float u, v;
        sscanf(line, "%s %f %f", token, &u, &v);
        texCoords.push_back(u);
        texCoords.push_back(v);
      }
      else if (strcmp(token, "vn") == 0)  // New normal.
      {
        float nx, ny, nz;
        sscanf(line, "%s %f %f %f", token, &nx, &ny, &nz);
        normals.push_back(nx);
        normals.push_back(ny);
        normals.push_back(nz);
      }
      else if (strcmp(token, "f") == 0)  // New face.
      {
        int a_v, a_vt, a_vn;
        int b_v, b_vt, b_vn;
        int c_v, c_vt, c_vn;

        if (sscanf(line, "%s %d//%d %d//%d %d//%d", token, &a_v, &a_vn, 
                                                           &b_v, &b_vn, 
                                                           &c_v, &c_vn) == 7)
        {
            // No texture coordinates
        }
        else if (sscanf(line, "%s %d/%d/%d %d/%d/%d %d/%d/%d", token, &a_v, &a_vt, &a_vn, 
                                                                      &b_v, &b_vt, &b_vn, 
                                                                      &c_v, &c_vt, &c_vn) == 9)
        {
          // Has texture coordinates.
          a_vt--;
          b_vt--;
          c_vt--;

          groupTexCoords.push_back(texCoords[2*a_vt + 0]);
          groupTexCoords.push_back(texCoords[2*a_vt + 1]);

          groupTexCoords.push_back(texCoords[2*b_vt + 0]);
          groupTexCoords.push_back(texCoords[2*b_vt + 1]);

          groupTexCoords.push_back(texCoords[2*c_vt + 0]);
          groupTexCoords.push_back(texCoords[2*c_vt + 1]);
        }

        a_v--; a_vn--;  
        b_v--; b_vn--;  
        c_v--; c_vn--;  

        // Vertex A.
        groupPositions.push_back(positions[3*a_v + 0]);
        groupPositions.push_back(positions[3*a_v + 1]);
        groupPositions.push_back(positions[3*a_v + 2]);
        groupNormals.push_back(normals[3*a_vn + 0]);
        groupNormals.push_back(normals[3*a_vn + 1]);
        groupNormals.push_back(normals[3*a_vn + 2]);

        // Vertex B.
        groupPositions.push_back(positions[3*b_v + 0]);
        groupPositions.push_back(positions[3*b_v + 1]);
        groupPositions.push_back(positions[3*b_v + 2]);
        groupNormals.push_back(normals[3*b_vn + 0]);
        groupNormals.push_back(normals[3*b_vn + 1]);
        groupNormals.push_back(normals[3*b_vn + 2]);

        // Vertex C.
        groupPositions.push_back(positions[3*c_v + 0]);
        groupPositions.push_back(positions[3*c_v + 1]);
        groupPositions.push_back(positions[3*c_v + 2]);
        groupNormals.push_back(normals[3*c_vn + 0]);
        groupNormals.push_back(normals[3*c_vn + 1]);
        groupNormals.push_back(normals[3*c_vn + 2]);
      }
      else
      {
        // Ignore anything else
      }
    }

    Object::BuildUpGroup(groupPositions, groupTexCoords, groupNormals, name);

    fclose(inFile);
    return true;
  }
  else  // Failed.
  {
    printf("ERROR Couldn't load file at %s\n", objFilepath.c_str());
    return false;
  }
}

bool Object::Load(std::function<glm::vec3 (float, float)> surf, 
  int numSampleU, int numSampleV)
{
  int w = numSampleU;
  int h = numSampleV;
  int numVertices = (w * h);
  int numIndices  = (2 * w * h);

  std::vector<GLfloat> vertices;
  std::vector<GLuint> indices; 

  vertices.reserve(numVertices * 6);
  indices.reserve(numIndices);

  // Initialize vertices.
  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      glm::vec3 surf_uv = surf(static_cast<float>(x)/(w-1), static_cast<float>(y)/(h-1));

      // Positions x, y, z.
      vertices.push_back(surf_uv[0]);
      vertices.push_back(surf_uv[1]);
      vertices.push_back(surf_uv[2]);

      // Colors RGB.
      vertices.push_back(0.6f);
      vertices.push_back(0.6f);
      vertices.push_back(0.6f);
    }
  }

  // Initialize indices.
  // Wire frame Element array - indices are written in a zig-zag pattern,
  // first horizontally and then vertically. It uses GL_LINE_STRIP. 
  int index = 0;
  int x = 0, y = 0;
  int dx = 1, dy = -1;

  for (y = 0; y < h; y++)  // Horizontally.
  {
    x = ((dx == 1) ? 0 : w-1);
    for (int k = 0; k < w; k++, x += dx)
      indices.push_back(INDEX(x, y));
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
      indices.push_back(INDEX(x, y));
    dy *= -1;
  }

  Mesh* mesh = new Mesh(mProgramHandle);
  mesh->Load(&vertices[0], &indices[0], numVertices, numIndices, true, false, false, GL_LINE_STRIP);
  mUsingLighting = false;
  mGroups.emplace_back(mesh, 0, "Main surface");
}

// ================= Intersect Ray =============== //

bool Object::IntersectRay(const glm::vec3& ray, const glm::vec3& C) const
{
  // TODO: walk through triangle array and compute intersection.
}

// ================= Destructor ================== //

Object::~Object()
{
  if (mOwnsData)
  {
    for (auto& group : mGroups)
    {
      delete group.mesh;
    }

    // TODO: check if deleting materials is needed.

  }
}

}  // namespace obj

}  // namespace gloo