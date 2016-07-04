#include "my_model.h"

#include <gloo/transform.h>
#include <gloo/shader_program.h>

#include <cstdio>
#include <iostream>

GLfloat triangleVertices[4][3] = {{-0.5f,  0.5f, 0.0f},
                                  { 0.5f,  0.5f, 0.0f},
                                  {-0.5f, -0.5f, 0.0f},
                                  { 0.5f, -0.5f, 0.0f}};

GLfloat triangleColors[] = {1.0f, 0.0f, 0.0f, 
                            0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 1.0f,
                            0.4f, 0.4f, 0.4f};

GLuint indices[] = {0, 2, 1, 3};

GLuint vbo = 0;  // Vertex buffer object.
GLuint vao = 0;  // Vertex array object.
GLuint eab = 0;  // Element array buffer.

gloo::ShaderProgram *shaderProgram;
GLuint program; 

MyModel::MyModel()
{
  
}

bool MyModel::Init()
{
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glPointSize(2);
  
  shaderProgram = new gloo::ShaderProgram();
  shaderProgram->BuildFromFiles("phong_no_shadow/vertex_shader.glsl", "phong_no_shadow/fragment_shader.glsl");
  shaderProgram->PrintCompilationLog();
  gloo::CompilationStatus status = shaderProgram->GetCompilationStatus();
  // std::cout << "Compilation Status = " << status << std::endl;

  if (status != gloo::CompilationStatus::kSuccess) 
  {
    delete shaderProgram;
    return false;
  }

  program = shaderProgram->GetHandle();
  shaderProgram->Bind();

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices) + sizeof(triangleColors), NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(triangleVertices), triangleVertices);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(triangleVertices), sizeof(triangleColors), triangleColors);

  glGenBuffers(1, &eab);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // get location index of the “position” shader variable
  GLuint locPositionAttrib = glGetAttribLocation(program, "in_position"); 
  GLuint locColorAttrib    = glGetAttribLocation(program, "in_color");

  glEnableVertexAttribArray(locPositionAttrib);
  glEnableVertexAttribArray(locColorAttrib);

  glVertexAttribPointer(locPositionAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(locColorAttrib, 3, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(triangleColors));

  return true;
}

void MyModel::Idle()
{
  glutPostRedisplay();
}

void MyModel::Display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindVertexArray(vao); // bind the VAO

  static float blah_angle = 0.0;
  blah_angle += 0.01;
  // GLfloat m[16];

  gloo::Transform MV;
  MV.Translate(+0.0f, 0.0f, -0.25f);
  MV.SetUniform(shaderProgram->GetHandle(), "V");
  // glUniformMatrix4fv( glGetUniformLocation(shaderProgram->GetHandle(), "V"), 1, GL_FALSE, m);

  MV.LoadIdentity();
  MV.PushMatrix();
  MV.Translate(-0.75f, 0.0f, 0.0f);
  MV.PushMatrix();
  MV.Rotate(blah_angle, 0, 0, 1);

  gloo::Transform copyMV = MV;

  // MV.PrintStack();

  // MV.Invert();
  // MV.Scale(0.5, 0.75, 1.0);

  // std::cout << copyMV << std::endl;

  GLuint uniformLoc = glGetUniformLocation(shaderProgram->GetHandle(), "M");
  copyMV.SetUniform(uniformLoc);
  // glUniformMatrix4fv( glGetUniformLocation(shaderProgram->GetHandle(), "M"), 1, GL_FALSE, m);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
  glDrawElements(
      GL_TRIANGLE_STRIP, // mode
      4,                 // count
      GL_UNSIGNED_INT,   // type
      (void*)0           // element array buffer offset
  );

  MV.LoadIdentity();
  //MV.Translate(+0.75f, 0.0f, 0.0f);
  // MV.PushAndLoadIdentity();
  MV.Rotate(blah_angle, 0, 0, 1);
  //MV.GetInverseMatrix(m);
  MV.SetInverseUniform(shaderProgram->GetHandle(), "M");
  // glUniformMatrix4fv( glGetUniformLocation(shaderProgram->GetHandle(), "M"), 1, GL_FALSE, m);
  
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  
  glutSwapBuffers();
}

void MyModel::Reshape(int w, int h)
{
  glViewport(0, 0, w, h);

  gloo::Transform P;

  GLfloat m[16];
  P.Ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.05f, 10.0f);
  // P.Perspective(60.0f, 4.0f/3.0f, 0.05f, 10.0f);
  // projectionMatrix.GetMatrix(m);
  P.GetMatrix(m);
  glUniformMatrix4fv( glGetUniformLocation(shaderProgram->GetHandle(), "P"), 1, GL_FALSE, m);
}

void MyModel::ActiveMouseMotion(int x, int y)
{

}

void MyModel::PassiveMouseMotion(int x, int y)
{

}

void MyModel::MouseButtonChange(int button, int state, int x, int y)
{

}

void MyModel::KeyboardChange(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 27: // ESC key
      exit(0);
    break;
  }
}

void MyModel::SpecialKeyboardChange(unsigned char key, int x, int y)
{

}