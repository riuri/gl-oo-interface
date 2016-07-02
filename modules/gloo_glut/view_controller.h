// + ======================================== +
// |         gl-oo-interface library          |
// |        Module: GLOO GLUT.                |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +
//
//  gloo::ViewController is a base class for a
//  view-controller design pattern for any app 
//  using GLUT. The ViewController contains a
//  gloo::Model pointer, which can be used for
//  a derived class of Model. Then, on your 
//  custom application, you should derive Mo-
//  del class and specify its instance to the 
//  ViewController, using SetModel() method.

#pragma once

#include "gl_header.h"

namespace gloo 
{

class Model;  // gloo::Model

class ViewController
{
public:
  ViewController();
  virtual ~ViewController();

  // Sets the gloo::Model to be called on every callback. 
  // By default, ViewController does not own the model,
  // so it means that the Model has to be manually destroyed.
  // If you want ViewController to automatically destroy model,
  // please use the second version of SetModel and specify true. 
  inline void SetModel(class Model* model) { mModel = model; }
  void SetModel(class Model* model, bool ownsModel);

  // Returns the pointer for the current model.
  inline class Model* GetModel() { return mModel; }

  // Tells if ViewController owns the strong reference to Model.
  inline bool OwnsModel() { return mOwnsModel; }

  // Tells if ViewController has a Model* initialized.
  inline bool HasModel() { return mModel != nullptr; }

  // -- Essential Callbacks --
  virtual void Idle();                 // Default callback in each cycle of GLUT.
  virtual void Display();              // Always called before rendering.
  virtual void Reshape(int w, int h);  // Called when the app window is resized.

  // -- Mouse Callbacks --
  virtual void  ActiveMouseMotion(int x, int y);  // When mouse moves when a button is pressed.
  virtual void PassiveMouseMotion(int x, int y);  // When mouse moves without any button pressed.
  virtual void MouseButtonChange(int button, int state, int x, int y);  // When buttons state changes.

  // -- Keyboard Callbacks --
  virtual void KeyboardChange(unsigned char key, int x, int y);         // Called when any key is pressed.
  virtual void SpecialKeyboardChange(unsigned char key, int x, int y);  // Called when special keys are pressed.

protected:
  class Model* mModel { nullptr };
  bool mOwnsModel { false };

};

inline void ViewController::SetModel(class Model* model, bool ownsModel)
{
  mModel = model;
  mOwnsModel = ownsModel;
}

}  // namespace gloo.










