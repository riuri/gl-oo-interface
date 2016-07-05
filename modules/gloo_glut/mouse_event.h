// + ======================================== +
// |         gl-oo-interface library          |
// |        Module: GLOO GLUT.                |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +
//
// This file provides structures for mouse events data. 
//
// MouseState contains all information about the current state of main 
// functionalities of a mouse, such as left, right and middle button 
// and also its coordinates.
// 
// MouseEvent contains the information that was changed
// during a mouse event and the new mouse state. Then,
// when implementing a mouse method handler for ModelBase,
// you can access the public data of MouseEvent to query
// about new position, as well as which buttons've changed.
// 
// If you want to use auxiliar keys (CTRL, SHIFT, ALT), you
// can check the member MouseState::mModifier, which tells
// what is the current modifier (key). The possible values 
// are kCTRL, kALT, kSHIFT or kNoMod.

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