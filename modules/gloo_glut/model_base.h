// + ======================================== +
// |         gl-oo-interface library          |
// |        Module: GLOO GLUT.                |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +
//
//  gloo::ModelBase is a base class for a model,
//  following the MVC design pattern in UI apps
//  using GLUT. The gloo::ViewController contains
//  a pointer to ModelBase instance. Therefore,
//  once you implement your custom Model class
//  derived from ModelBase, you will set it to be
//  the model in gloo::ViewController (please, 
//  use the method SetModel() and see ViewController
//  for more details).
//

#pragma once

#include "gl_header.h"

namespace gloo
{

class ModelBase
{
public:
  // Virtual destructor.
  virtual ~ModelBase();

  // -- Essential Callbacks --
  virtual void Idle() = 0;       // Default callback in each cycle of GLUT.
  virtual void Display() = 0;    // Always called before rendering.
  virtual void Reshape(int w, int h) = 0;  // Called when the app window is resized.

  // -- Mouse Callbacks --
  virtual void  ActiveMouseMotion(int x, int y) = 0;  // Mouse moves while a button is pressed.
  virtual void PassiveMouseMotion(int x, int y) = 0;  // Mouse moves with no buttons pressed.
  virtual void MouseButtonChange(int button, int state, int x, int y) = 0;  // Buttons state changes.

  // -- Keyboard Callbacks --
  virtual void KeyboardChange(unsigned char key, int x, int y) = 0;  // Normal ASCII keys pressed.
  virtual void SpecialKeyboardChange(unsigned char key, int x, int y) = 0;  // Special keys pressed.
};




}  // namespace gloo.