// + ======================================== +
// |         gl-oo-interface library          |
// |        Module: GLOO GLUT.                |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +
//
// GlutApplication provides an interface made up of static 
// functions to help creating a single-window glut applica-
// tion.
// 
// The use is very simple: just call the method Run() and
// pass argc and argv from main(), the ModelBase-derived
// class instance (and optionally the window title and size).
// Run() returns similar int codes to main() in any application. 
// See the glut_app example in the folder "../../examples/glut_app".
//

#pragma once

#include <string>

#include "model_base.h"
#include "glut_view_controller.h"

namespace gloo
{

const unsigned int _kDefaultDisplayMode = (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);

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
                 int windowWidth = 800, int windowHeight = 600,
                 unsigned int displayMode = _kDefaultDisplayMode);

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
