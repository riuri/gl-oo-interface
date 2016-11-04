// + ======================================== +
// |         gl-oo-interface library          |
// |        Module: GLOO GLUT.                |
// |        Author: Rodrigo Castiel, 2016.    |
// + ======================================== +
//
//  gloo::GlutViewController is a base class for a
//  view-controller design pattern for any app 
//  using GLUT. The GlutViewController contains a
//  gloo::ModelBase pointer, which can be used for
//  a derived class of ModelBase. Then, on your 
//  custom application, you should derive Mo-
//  del class and specify its instance to the 
//  GlutViewController, using SetModel() method.

#pragma once

#include "../include/gloo/gl_header.h"
#include "mouse_event.h"

namespace gloo 
{

class ModelBase;  // gloo::ModelBase

// Base class for view controller (MVC paradigm).
class GlutViewController
{
public:
  GlutViewController();
  virtual ~GlutViewController();

  bool Init();

  // Sets the gloo::ModelBase to be called on every callback. 
  // By default, GlutViewController does not own the model,
  // so it means that the ModelBase has to be manually destroyed.
  // If you want GlutViewController to automatically destroy model,
  // please use the second version of SetModel and specify true. 
  inline void SetModel(class ModelBase* model) { mModel = model; }
  void SetModel(class ModelBase* model, bool ownsModel);

  // Returns the pointer for the current model.
  inline class ModelBase* GetModel() const { return mModel; }

  // Tells if GlutViewController owns the strong reference to ModelBase.
  inline bool OwnsModel() const { return mOwnsModel; }

  // Tells if GlutViewController has a ModelBase* initialized.
  inline bool HasModel() const { return mModel != nullptr; }

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
  class ModelBase* mModel { nullptr };
  bool mOwnsModel { false };

  MouseState mMouseState;
};

// =========== GlutViewController Inline Methods =============== 

inline void GlutViewController::SetModel(class ModelBase* model, bool ownsModel)
{
  mModel = model;
  mOwnsModel = ownsModel;
}

}  // namespace gloo.










