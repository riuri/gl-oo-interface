#include "my_model.h"

#include <gloo/transform.h>
#include <gloo/mouse_event.h>
#include <gloo/group.h>

#include <cstdio>
#include <iostream>


GLfloat squareVertices[] = {-1.5f, 0.0f,  1.5f,
                             1.5f, 0.0f,  1.5f,
                            -1.5f, 0.0f, -1.5f,
                             1.5f, 0.0f, -1.5f};

GLfloat squareColors[] = {1.0f, 0.0f, 0.0f, 
                          0.0f, 1.0f, 0.0f,
                          0.0f, 0.0f, 1.0f,
                          0.4f, 0.4f, 0.4f};

GLfloat squareUV[] = { 0.0f, 0.0f,
                       1.0f, 0.0f,
                       0.0f, 1.0f,
                       1.0f, 1.0f };

GLfloat squareNormals[] = {0.0f, 1.0f, 0.0f, 
                           0.0f, 1.0f, 0.0f,
                           0.0f, 1.0f, 0.0f,
                           0.0f, 1.0f, 0.0f};


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

  delete mDebugRenderer;
  delete mPhongRenderer;

  delete mAxis;
  delete mGrid;
  delete mPolygon;
  delete mBoundingBox;
  delete mWireframeSphere;

  delete mLightSource;
}

bool MyModel::Init()
{
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  mDebugRenderer = new DebugRenderer();
  mPhongRenderer = new PhongRenderer();

  if (!mDebugRenderer->Load())
  {
    std::cout << "Couldn't initialize 'MyModel::DebugRenderer*' ..." << std::endl;
    delete mDebugRenderer;
    return false;
  }

  if (!mPhongRenderer->Load())
  {
    std::cout << "Couldn't initialize 'MyModel::PhongRenderer*' ..." << std::endl;
    delete mPhongRenderer;
    return false;
  }


  mPhongRenderer->SetNumLightSources(1);

  mCamera = new Camera();
  mCamera->SetPosition(0, 0, 3.0f);

  GLint posAttribLoc  = mDebugRenderer->GetPositionAttribLoc();
  GLint colAttribLoc  = mDebugRenderer->GetColorAttribLoc();

  mAxis = new AxisMesh(posAttribLoc, colAttribLoc);
  mGrid = new GridMesh(posAttribLoc, colAttribLoc, 9, 9, 0.15f, {0.4f, 0.4f, 0.4f});
  mBoundingBox = new BoundingBoxMesh(posAttribLoc, colAttribLoc);
  mPolygon = new Polygon(posAttribLoc, colAttribLoc, 1.0f, 10);
  mWireframeSphere = new WireframeSphere(posAttribLoc, colAttribLoc, {0.0f, 1.0f, 0.0f}, 16);


  GLint posAttribLocPhong = mPhongRenderer->GetPositionAttribLoc();
  GLint normalAttribLocPhong =  mPhongRenderer->GetNormalAttribLoc();
  GLint textureAttribLocPhong = mPhongRenderer->GetTextureAttribLoc();

  mMeshGroup = new MeshGroup<Batch>(4, 4);
  mMeshGroup->SetVertexAttribList({3, 3, 2});
  mMeshGroup->AddRenderingPass({{posAttribLoc, true}, {colAttribLoc, true}, gloo::kNoAttrib});
  mMeshGroup->AddRenderingPass({{posAttribLocPhong, true}, {normalAttribLocPhong, true}, {textureAttribLocPhong, true}});

  // mMeshGroup->Load(squareBuffer, nullptr);
  mMeshGroup->Load({squareVertices, squareNormals, squareUV}, nullptr);

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

  gloo::Transform M;
  M.LoadIdentity();

  mPhongRenderer->Bind();
  mPhongRenderer->SetCamera(mCamera);

  LightSource lightSource = { glm::vec3(0,    1,  0),        // Pos.
                              glm::vec3(0,   -1,  0),  // Dir.
                              glm::vec3(0.7, 0.7, 1),  // Ld.
                              glm::vec3(1.0, 1.0, 1),  // Ls.
                              5.0f*std::cos(0*blah_angle*1.9f)+10.0f};  // Alpha.

  mPhongRenderer->EnableLightSource(0);
  mPhongRenderer->SetLightSourceInCameraCoordinates(lightSource, mCamera, 0);

  mPhongRenderer->SetMaterial({ glm::vec3(0,  0, 0), 
                                glm::vec3(.8, 0, 0),
                                glm::vec3(.2, .2, .2)});
  
  M.LoadIdentity();
  M.Rotate(-0.79*cos(blah_angle), 1, 0, 1);
  mPhongRenderer->Render(mMeshGroup, M, 1);
  M.LoadIdentity();


  if (mRendererNum == 1) 
  {
    mDebugRenderer->Bind();
    mDebugRenderer->Render(mGrid->GetMeshGroup(), M, mCamera);
    mDebugRenderer->Render(mAxis->GetMeshGroup(), M, mCamera);
    mDebugRenderer->Render(mWireframeSphere->GetMeshGroup(), M, mCamera);

    M.Rotate(-0.79*blah_angle, 0, 1, 0);
    M.Translate(-1.2f, 0.0f, 0.0f);
    M.Rotate(blah_angle, 0, 0, 1);
    M.Scale(0.25f, 0.25f, 0.25f);
    mDebugRenderer->Render(mWireframeSphere->GetMeshGroup(), M, mCamera);
  }

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
        mCamera->Translate(0.0f, 0.0f, -mouseEvent.mMouseState.mVelY * 1e-2);
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

    case 'u':
    case 'U':
      mRendererNum = (mRendererNum+1) % 2;
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