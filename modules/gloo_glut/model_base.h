// + ======================================== +
// |         gl-oo-interface library          |
// |        Module: GLOO GLUT.                |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +
//
//  gloo::ModelBase is a base class for a model,
//  following the MVC design pattern in UI apps
//  Each gloo::ViewController-like class contains
//  a pointer to ModelBase instance. Therefore,
//  once you implement your custom Model class
//  derived from ModelBase, you will set it to be
//  the model in corresponding view-controller 
//  (please, use the method SetModel()).


#pragma once

#include "../include/gloo/gl_header.h"

namespace gloo
{

class ModelBase
{
public:
  // Virtual destructor.
  virtual ~ModelBase() {}

  // -- Initialization method --
  virtual bool Init() = 0;

  // -- Essential Callbacks --
  virtual void Idle() = 0;       // Default callback in each cycle of GLUT.
  virtual void Display() = 0;    // Always called before rendering.
  virtual void Reshape(int w, int h) = 0;  // Called when the app window is resized.

  // -- Mouse Callbacks --
  virtual void  ActiveMouseMotion(const struct MouseEvent & mouseEvent) = 0;  // Mouse moves while a button is pressed.
  virtual void PassiveMouseMotion(const struct MouseEvent & mouseEvent) = 0;  // Mouse moves with no buttons pressed.
  virtual void  MouseButtonChange(const struct MouseEvent & mouseEvent) = 0;  // Buttons state changes.

  // -- Keyboard Callbacks --
  virtual void KeyboardChange(unsigned char key, int x, int y) = 0;  // Normal ASCII keys pressed.
  virtual void SpecialKeyboardChange(unsigned char key, int x, int y) = 0;  // Special keys pressed.
};




}  // namespace gloo.