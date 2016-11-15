#include "my_model.h"

#include <gloo/transform.h>
#include <gloo/mouse_event.h>
#include <gloo/group.h>

#include <cstdio>
#include <iostream>


GLfloat squareVertices[] = {-0.5f, 0.0f,  0.5f,
                             0.5f, 0.0f,  0.5f,
                            -0.5f, 0.0f, -0.5f,
                             0.5f, 0.0f, -0.5f};

GLfloat squareColors[] = {1.0f, 0.0f, 0.0f, 
                          0.0f, 1.0f, 0.0f,
                          0.0f, 0.0f, 1.0f,
                          0.4f, 0.4f, 0.4f};

GLfloat squareNormals[] = {1.0f, 0.0f, 0.0f, 
                           0.0f, 1.0f, 0.0f,
                           0.0f, 0.0f, 1.0f,
                           0.4f, 0.4f, 0.4f};


GLfloat squareBuffer[] = {-0.5f, 0.0f,  0.5f,
                           0.5f, 0.0f,  0.5f,
                          -0.5f, 0.0f, -0.5f,
                           0.5f, 0.0f, -0.5f,

                          1.0f, 0.0f, 0.0f,
                          0.0f, 1.0f, 0.0f,
                          0.0f, 0.0f, 1.0f,
                          0.4f, 0.4f, 0.4f
                           };


GLfloat squareBufferInterleaved[] =  {-0.5f, 0.0f,  0.5f,   1.0f, 0.0f, 0.0f,
                                       0.5f, 0.0f,  0.5f,   0.0f, 1.0f, 0.0f,
                                      -0.5f, 0.0f, -0.5f,   0.0f, 0.0f, 1.0f,
                                       0.5f, 0.0f, -0.5f,   0.4f, 0.4f, 0.4f};

GLuint indices[] = {0, 2, 1, 3};

GLuint program; 

MyModel::MyModel()
{

}

MyModel::~MyModel()
{
  delete mCamera;
  delete mMeshGroup;
  delete mMeshGroup2;

  delete mDebugRenderer;

  delete mAxis;
  delete mGrid;
  delete mPolygon;
  delete mBoundingBox;

  delete mLightSource;
}

bool MyModel::Init()
{
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glPointSize(2);

  mDebugRenderer = new DebugRenderer();

  if (!mDebugRenderer->Load())
  {
    std::cout << "Couldn't initialize 'MyModel::DebugRenderer*' ..." << std::endl;
    delete mDebugRenderer;
    return false;
  }

  mCamera = new Camera();
  mCamera->SetPosition(0, 0, 3.0f);

  GLint posAttribLoc  = mDebugRenderer->GetPositionAttribLoc();
  GLint colAttribLoc  = mDebugRenderer->GetColorAttribLoc();

  mAxis = new AxisMesh(posAttribLoc, colAttribLoc);
  mGrid = new GridMesh(posAttribLoc, colAttribLoc, 9, 9, 0.15f, 0.4f, 0.4f, 0.4f);
  mBoundingBox = new BoundingBoxMesh(posAttribLoc, colAttribLoc);
  mPolygon = new Polygon(posAttribLoc, colAttribLoc, 1.0f, 10);

  mMeshGroup = new MeshGroup<Batch>(4, 4);
  mMeshGroup2 = new MeshGroup<Interleave>(4, 4);
  
  mMeshGroup->SetVertexAttribList({3, 3});

  mMeshGroup->AddRenderingPass({{posAttribLoc, true}, {colAttribLoc, true}});
  mMeshGroup->Load(squareBuffer, nullptr);

  mMeshGroup2->SetVertexAttribList({3, 3});
  mMeshGroup2->AddRenderingPass({{posAttribLoc, true}, {colAttribLoc, true}});
  mMeshGroup2->Load( {squareVertices, squareColors}, nullptr);

  return true;
}

void MyModel::Idle()
{
  glutPostRedisplay();
}

void MyModel::Display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  static float blah_angle = 0.0;
  blah_angle += 0.01;

  mCamera->SetOnRendering();
  mDebugRenderer->Bind();

  gloo::Transform M;
  M.LoadIdentity();
  M.Translate(1.2f, 0.0f, 0.0);

  M.LoadIdentity();
  M.Rotate(-0.79*blah_angle, 0, 1, 0);
  M.Translate(-1.2f, 0.0f, 0.0f);
  M.Rotate(blah_angle, 0, 0, 1);

  mDebugRenderer->Render(mBoundingBox->GetMeshGroup(), M, mCamera);
  mDebugRenderer->Render(mMeshGroup2, M, mCamera);
  
  M.LoadIdentity();

  mDebugRenderer->Render(mGrid->GetMeshGroup(), M, mCamera);
  mDebugRenderer->Render(mAxis->GetMeshGroup(), M, mCamera);
  mDebugRenderer->Render(mMeshGroup, M, mCamera);

  glutSwapBuffers();
}

void MyModel::Reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  mCamera->SetOnReshape(0, 0, w, h);
}

void MyModel::ActiveMouseMotion(const MouseEvent & mouseEvent)
{
  switch (mouseEvent.mMouseState.mModifier)
  {
    case MouseState::kCTRL:

    break;

    case MouseState::kSHIFT:

    break;

    case MouseState::kALT:

    break;
  
    default:
      if (mouseEvent.mMouseState.mLftButton == MouseState::kDown)
      {
        // std::cout << "Left button dragged.\n";
      }
      if (mouseEvent.mMouseState.mRgtButton == MouseState::kDown)
      {
        mCamera->Rotate(mouseEvent.mMouseState.mVelY/100.0f, 
                        mouseEvent.mMouseState.mVelX/100.0f,
                        0.0f);
        // std::cout << "Right button dragged.\n";
      }
      if (mouseEvent.mMouseState.mMidButton == MouseState::kDown)
      {
        // std::cout << "dy = " << mouseEvent.mMouseState.mVelY * 1e-2 << std::endl;
        mCamera->Translate(0.0f, 0.0f, -mouseEvent.mMouseState.mVelY * 1e-2);
        // std::cout << "Middle button dragged.\n";
      }
  }  // end switch.

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
    // std::cout << "Left button pressed.\n";
  }
  if (mouseEvent.mButton == MouseState::kRight && mouseEvent.mButtonState == MouseState::kUp)
  {
    // std::cout << "Right button pressed.\n";
  }
  if (mouseEvent.mButton == MouseState::kMiddle && mouseEvent.mButtonState == MouseState::kUp)
  {
    // std::cout << "Middle button pressed.\n";
  }
}

void MyModel::KeyboardChange(unsigned char key, int x, int y)
{
  static bool fullScreen = false;

  switch (key)
  {
    case 27: // ESC key
      exit(0);
    break;

    case 'F':
    case 'f':
      if (fullScreen)
      {
        glutReshapeWindow(800, 600);
        glutPositionWindow(0, 0);
      }
      else
      {
        glutFullScreen();
      }
      fullScreen = !fullScreen;
    break;
  }
}

void MyModel::SpecialKeyboardChange(unsigned char key, int x, int y)
{

}