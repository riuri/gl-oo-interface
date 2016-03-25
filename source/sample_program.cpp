#include "sample_program.h"

SampleProgram::SampleProgram() : GlutProgram()
{
  mScene = new Scene();
  mVideoRecorder = new VideoRecorder();
}

void SampleProgram::Init(int* argc, char* argv[], const char* windowTitle)
{
  GlutProgram::Init(argc, argv, windowTitle);
  GlutProgram::LoadShaders("./shaders/");
  SampleProgram::InitScene(*argc, argv);
}

void SampleProgram::InitScene(int argc, char *argv[])
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT,  GL_NICEST);
  
  mScene->Init(mPipelineProgram, mProgramHandle);

  // Insert new objects here!!
  AxisObject* originAxis = new AxisObject(mPipelineProgram, mProgramHandle);
  GridObject* originGrid = new GridObject(mPipelineProgram, mProgramHandle);
  TexturedSphere* sphere = new TexturedSphere(mPipelineProgram, mProgramHandle);
  TexturedTerrain* terrain = new TexturedTerrain(mPipelineProgram, mProgramHandle);
  Light* l1 = new Light(mPipelineProgram, mProgramHandle);
  Light* l2 = new Light(mPipelineProgram, mProgramHandle);
  Light* l3 = new Light(mPipelineProgram, mProgramHandle);
  Light* l4 = new Light(mPipelineProgram, mProgramHandle);

  l1->SetPosition(glm::vec3(+50, 50, +50)); 
  l2->SetPosition(glm::vec3(+50, 50, -50));
  l3->SetPosition(glm::vec3(-50, 50, +50));
  l4->SetPosition(glm::vec3(-50, 50, -50));

  originAxis->Load();
  originGrid->Load(100, 100);
  sphere->Load("textures/earth.jpg");
  terrain->Load("", "textures/plaster_tile.jpg");
  terrain->SetLighting(false);
  terrain->SetScale(100, 1, 100);

  mScene->Add(sphere);
  mScene->Add(originAxis);
  mScene->Add(originGrid);
  mScene->Add(terrain);
  
  mScene->Add(l1);
  mScene->Add(l2);
  mScene->Add(l3);
  mScene->Add(l4);
}

// GLUT Callback methods ----------------------------------------------------------------

void SampleProgram::DisplayFunc()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  mScene->Render();
  glutSwapBuffers();
  mVideoRecorder->Update();
}

void SampleProgram::IdleFunc()
{
  mScene->Animate();
  glutPostRedisplay();
}

void SampleProgram::ReshapeFunc(int w, int h)
{
  GlutProgram::ReshapeFunc(w, h);

  mScene->ReshapeScreen(w, h);
  mVideoRecorder->UpdateSize(w, h);
}

void SampleProgram::PassiveMotionFunc(int x, int y)
{
  GlutProgram::PassiveMotionFunc(x, y);
}

void SampleProgram::MouseFunc(int button, int state, int x, int y)
{
  GlutProgram::MouseFunc(button, state, x, y);

  // NOTE: The following code was provided by the starter code.
  // keep track of whether CTRL and SHIFT keys are pressed
  switch (glutGetModifiers())
  {
    case GLUT_ACTIVE_ALT:
        mControlState = kEDIT;

        if (mMouse.mLftButton)
        {
          mScene->SelectObject(x, y, mWindowWidth, mWindowHeight);
        }
        // if (mMouse.mLftButton) // Clicking with left button in EDIT mode
        // {
        //   mScene->OnMouseLeftClick(x, y, mWindowWidth, mWindowHeight);
        // }
        // else if (mMouse.mRgtButton) // Clicking with left button in EDIT mode
        // {
        //   mScene->OnMouseRightClick(x, y, mWindowWidth, mWindowHeight);
        // }

    break;

    case GLUT_ACTIVE_CTRL:
      mControlState = kTRANSLATE;
    break;

    case GLUT_ACTIVE_SHIFT:
      mControlState = kSCALE;
    break;

    // if CTRL and SHIFT are not pressed, we are in rotate mode
    default:
      mControlState = kROTATE;
    break;
  }
  // END.

}

// Drag function.
void SampleProgram::MotionFunc(int x, int y)
{
  Camera* camera = mScene->GetCurrentCamera();

  int mousePosDelta[2] = { x - mMouse.mPos[0], y - mMouse.mPos[1] };

  switch (mControlState)
  {
    // deform landscape
    case kEDIT:
      break;

    // translate the landscape
    case kTRANSLATE:
      if (mMouse.mLftButton)
      {
        camera->Translate(-mousePosDelta[0]/40.0f, +mousePosDelta[1]/10.0f, 0.0f);
      }
      if (mMouse.mRgtButton)
      {
        // control z translation via the right mouse button
        camera->Translate(0.0f, 0.0f, +mousePosDelta[1]/10.0f);
      }
      break;

    // rotate the landscape
    case kROTATE:
      if (mMouse.mLftButton)
      {
        camera->Rotate(-mousePosDelta[1]/100.0f, -mousePosDelta[0]/100.0f, 0.0f);
      }
      if (mMouse.mRgtButton)
      {
        // control z rotation via the right mouse button
        camera->Rotate(0.0f, 0.0f, mousePosDelta[1]/100.0f);
      }
      break;

    // scale the landscape
    case kSCALE:
      if (mMouse.mLftButton)
      {
        camera->Scale(+mousePosDelta[0]/100.0f, -mousePosDelta[1]/100.0f, 0.0f);
      }
      if (mMouse.mRgtButton)
      {
        // control z scaling via the right mouse button
        camera->Scale(0.0f, 0.0f, -mousePosDelta[1]/100.0f);
      }
      break;
  }
  // END.

  GlutProgram::MotionFunc(x, y);
}

void SampleProgram::KeyboardFunc(unsigned char key, int x, int y)
{
  GlutProgram::KeyboardFunc(key, x, y);

  switch (key)
  {
    case ' ':
      std::cout << "You pressed the spacebar." << std::endl;
      mVideoRecorder->ToggleRecord();
    break;

    case 'c':
      mScene->ChangeCamera();
    break;

    case 'f':
      glutFullScreen();
    break;

    case 'x':
      // take a screenshot
      mVideoRecorder->TakeScreenshot();
    break;
  }
}

// GLUT Callback methods end ------------------------------------------------------------