#include "my_model.h"

#include <gloo/transform.h>
#include <gloo/mouse_event.h>

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

GLuint program; 

MyModel::MyModel()
{

}

MyModel::~MyModel()
{
  delete mCamera;
  delete mShaderProgram;
}

bool MyModel::Init()
{
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glPointSize(2);
  
  mShaderProgram = new gloo::ShaderProgram();
  mShaderProgram->BuildFromFiles("phong_no_shadow/vertex_shader.glsl", 
                                 "phong_no_shadow/fragment_shader.glsl");
  mShaderProgram->PrintCompilationLog();
  gloo::CompilationStatus status = mShaderProgram->GetCompilationStatus();
  std::cout << "Compilation Status = " << status << std::endl;

  if (status != gloo::CompilationStatus::kSuccess) 
  {
    delete mShaderProgram;
    return false;
  }

  program = mShaderProgram->GetHandle();
  mShaderProgram->Bind();

  mCamera = new Camera();
  mCamera->SetPosition(0, 0, 1.0f);

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

  mCamera->Set();
  mCamera->SetUniformViewMatrix( mShaderProgram->GetVariableHandle("V") );
  // mCamera->SetUniformViewProj( mShaderProgram->GetVariableHandle("PV") );

  /* Scale, Rotate, Translate method of camera */
  // mCamera->Scale(-0.01, -0.004, -0.004);
  // mCamera->Translate(0, 0, 0.001f);
  mCamera->SetPosition(0, 0, 2);
  // mCamera->Rotate(0, 0, 0.04f);
  // mCamera->Rotate(0.01f, 0, 0);

  gloo::Transform M;
  M.LoadIdentity();
  M.Translate(0.75f, 0.0f, 0.0);
  // M.Rotate(blah_angle, 0, 0, 1);

  GLuint uniformLoc = glGetUniformLocation(mShaderProgram->GetHandle(), "M");
  M.SetUniform(uniformLoc);

  // mShaderProgram->GetVariableHandle("MV")
  // mCamera->SetUniformModelView( glGetUniformLocation(mShaderProgram->GetHandle(), "MV"), M);
  // mCamera->SetUniformModelViewProj( glGetUniformLocation(mShaderProgram->GetHandle(), "MVP"), M);

  // glUniformMatrix4fv( glGetUniformLocation(mShaderProgram->GetHandle(), "M"), 1, GL_FALSE, m);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
  glDrawElements(
      GL_TRIANGLE_STRIP, // mode
      4,                 // count
      GL_UNSIGNED_INT,   // type
      (void*)0           // element array buffer offset
  );

  M.LoadIdentity();
  M.Translate(-0.75f, 0.0f, 0.0f);
  M.Rotate(blah_angle, 0, 0, 1);

  M.SetUniform(uniformLoc);

  // mCamera->SetUniformModelView( glGetUniformLocation(mShaderProgram->GetHandle(), "MV"), M);
  // mCamera->SetUniformModelViewProj( glGetUniformLocation(mShaderProgram->GetHandle(), "MVP"), M);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  
  glutSwapBuffers();
}

void MyModel::Reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  mCamera->OnReshape(0, 0, w, h);

  mCamera->SetUniformProjMatrix( mShaderProgram->GetVariableHandle("P") );
}

void MyModel::ActiveMouseMotion(const MouseEvent & mouseEvent)
{

}

void MyModel::PassiveMouseMotion(const MouseEvent & mouseEvent)
{
  // std::cout << "Coordinates: " << mouseState.mPosX << ", " << mouseState.mPosY << std::endl;
}

void MyModel::MouseButtonChange(const MouseEvent & mouseEvent)
{
  if (mouseEvent.mMouseState.mModifier == MouseState::kCTRL)
  {
    std::cout << "CTRL.\n";
  }
  if (mouseEvent.mMouseState.mModifier == MouseState::kSHIFT)
  {
    std::cout << "SHIFT.\n";
  }
  if (mouseEvent.mMouseState.mModifier == MouseState::kALT)
  {
    std::cout << "ALT.\n";
  }

  if (mouseEvent.mButton == MouseState::kLeft && mouseEvent.mButtonState == MouseState::kUp)
  {
    std::cout << "Left button pressed.\n";
  }
  if (mouseEvent.mButton == MouseState::kRight && mouseEvent.mButtonState == MouseState::kUp)
  {
    std::cout << "Right button pressed.\n";
  }
  if (mouseEvent.mButton == MouseState::kMiddle && mouseEvent.mButtonState == MouseState::kUp)
  {
    std::cout << "Middle button pressed.\n";
  }
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