// + ======================================== +
// |         gl-oo-interface library          |
// |        Module: GLOO GLUT.                |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +

#pragma once

#include "gl_header.h"

namespace gloo
{

// =========================================================

// State machine for mouse.
// mPosX and mPosY define viewport coordinates of the mouse.
struct MouseState
{
  enum Modifier { kCTRL, kSHIFT, kALT, kNoMod };
  enum Button { kLeft, kRight, kMiddle, kNone };
  enum State  { kUp, kDown };

  // Current (x, y) coordinates of mouse (viewport).
  int mPosX { 0 };
  int mPosY { 0 };

  // State of each button.
  State mLftButton { kUp };
  State mRgtButton { kUp };
  State mMidButton { kUp };

  // Key modifier (GLUT_CTRL, GLUT_ALT, ...)
  Modifier mModifier { kNoMod };

  void SetPos(int x, int y);
  void SetGlutState(int button, int state);
};

// =========== MouseState Inline Methods =============== 

inline void MouseState::SetPos(int x, int y)
{
  mPosX = x;
  mPosY = y;
}

// =========================================================

// MouseEvent defines and stores information about any mouse
// event. button specifies the button that just changed and
// state specifies what is the new state of this button.
struct MouseEvent
{ 
  MouseState mMouseState;
  MouseState::Button mButton;
  MouseState::State mButtonState;
};


}  // namespace gloo.