// + ======================================== +
// |         gl-oo-interface library          |
// |        Module: GLOO GLUT.                |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +
//
// TODO: description.

#pragma once

#include <string>

#include "model_base.h"
#include "glut_view_controller.h"

namespace gloo
{

class GlutApplication
{
public:
  
  // Initializes OpenGL and GLUT context according to OS.
  // Builds internal GlutViewController.
  // Starts MainLoop of GlutApplication.
  // Exits return a code - 0 for success, 1 for error and so on.
  // Destroys data of GlutApplication.
  static int Run(int argc, char* argv[], ModelBase* model,
                 const std::string & windowTitle = "",
                 int windowWidth = 800, int windowHeight = 600);

private:
  static GlutViewController* sViewController;

  // GLUT Callbacks.
  static void Idle();     // Default callback in each cycle of GLUT. 
  static void Display();  // Always called before rendering.
  static void Reshape(int w, int h);   // Called when the app window is resized. 
  static void ActiveMouseMotion(int x, int y);    // When mouse moves when a button is pressed. 
  static void PassiveMouseMotion(int x, int y);   // When mouse moves without any button pressed.
  static void MouseButtonChange(int button, int state, int x, int y);  // When buttons state changes.
  static void KeyboardChange(unsigned char key, int x, int y);   // Called when any key is pressed.
  static void SpecialKeyboardChange(int key, int x, int y);      // Called when special keys are pressed.
};



}  // namespace gloo.
