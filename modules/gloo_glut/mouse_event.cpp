#include "mouse_event.h"

namespace gloo
{

void MouseState::SetGlutState(int button, int state)
{
  if (button == GLUT_LEFT_BUTTON)
  {
    mLftButton = (state == GLUT_DOWN ? State::kDown : State::kUp);
  }
  else if (button == GLUT_RIGHT_BUTTON)
  {
    mRgtButton = (state == GLUT_DOWN ? State::kDown : State::kUp);
  }
  else
  {
    mMidButton = (state == GLUT_DOWN ? State::kDown : State::kUp);
  }

  int modifier = glutGetModifiers();
  if (modifier == GLUT_ACTIVE_CTRL)
  {
    mModifier = Modifier::kCTRL;
  }
  else if (modifier == GLUT_ACTIVE_SHIFT)
  {
    mModifier = Modifier::kSHIFT;
  }
  else if (modifier == GLUT_ACTIVE_ALT)
  {
    mModifier = Modifier::kALT;
  }
  else
  {
    mModifier = Modifier::kNoMod;
  }
}

}  // namespace gloo.