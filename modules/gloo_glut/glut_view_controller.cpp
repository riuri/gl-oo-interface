#include "glut_view_controller.h"
#include "model_base.h"

namespace gloo
{
 
GlutViewController::GlutViewController()
{

}

GlutViewController::~GlutViewController()
{
  if (HasModel() && OwnsModel())
  {
    delete mModel;
  }
}

bool GlutViewController::Init()
{
  if (HasModel())
  {
    return mModel->Init();
  }

  return false;
}

void GlutViewController::Idle()
{
  if (HasModel())
  {
    mModel->Idle();
  }
}

void GlutViewController::Display()
{
  if (HasModel())
  {
    mModel->Display();
  }
}

void GlutViewController::Reshape(int w, int h)
{
  if (HasModel())
  {
    mModel->Reshape(w, h);
  }
}

void GlutViewController::ActiveMouseMotion(int x, int y)
{
  mMouseState.SetPos(x, y);

  if (HasModel())
  {
    MouseEvent event = { mMouseState, MouseState::kNone, MouseState::kDown };
    mModel->ActiveMouseMotion( event );
  }
}

void GlutViewController::PassiveMouseMotion(int x, int y)
{
  mMouseState.SetPos(x, y);

  if (HasModel())
  {
    MouseEvent event = { mMouseState, MouseState::kNone, MouseState::kUp };
    mModel->PassiveMouseMotion( event );
  }
}

void GlutViewController::MouseButtonChange(int button, int state, int x, int y)
{
  mMouseState.SetPos(x, y);
  mMouseState.SetGlutState(button, state);

  MouseState::State buttonState = (state == GLUT_DOWN ? MouseState::kDown : MouseState::kUp);

  MouseEvent event;
  event.mButtonState = buttonState;
  event.mMouseState = mMouseState;

  if (button == GLUT_LEFT_BUTTON)
  {
    event.mButton = MouseState::kLeft;
  }
  else if (button == GLUT_RIGHT_BUTTON)
  {
    event.mButton = MouseState::kRight;
  }
  else
  {
    event.mButton = MouseState::kMiddle;
  }

  if (HasModel())
  {
    mModel->MouseButtonChange( event );
  }
}

void GlutViewController::KeyboardChange(unsigned char key, int x, int y)
{
  if (HasModel())
  {
    mModel->KeyboardChange(key, x, y);
  }
}

void GlutViewController::SpecialKeyboardChange(unsigned char key, int x, int y)
{
  if (HasModel())
  {
    mModel->SpecialKeyboardChange(key, x, y);
  }
}

}  // namespace gloo.