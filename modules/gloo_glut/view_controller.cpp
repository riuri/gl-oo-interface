#include "view_controller.h"
#include "model_base.h"

namespace gloo
{
 
ViewController::ViewController()
{

}

ViewController::~ViewController()
{
  if (ViewController::HasModel() && ViewController::OwnsModel())
  {
    delete mModel;
  }
}

void ViewController::Idle()
{
  if (ViewController::HasModel())
  {
    mModel->Idle();
  }
}

void ViewController::Display()
{
  if (ViewController::HasModel())
  {
    mModel->Display();
  }
}

void ViewController::Reshape(int w, int h)
{
  if (ViewController::HasModel())
  {
    mModel->Reshape(w, h);
  }
}

void ViewController::ActiveMouseMotion(int x, int y)
{
  if (ViewController::HasModel())
  {
    mModel->ActiveMouseMotion(x, y);
  }
}

void ViewController::PassiveMouseMotion(int x, int y)
{
  if (ViewController::HasModel())
  {
    mModel->PassiveMouseMotion(x, y);
  }
}

void ViewController::MouseButtonChange(int button, int state, int x, int y)
{
  if (ViewController::HasModel())
  {
    mModel->MouseButtonChange(button, state, x, y);
  }
}

void ViewController::KeyboardChange(unsigned char key, int x, int y)
{
  if (ViewController::HasModel())
  {
    mModel->KeyboardChange(key, x, y);
  }
}

void ViewController::SpecialKeyboardChange(unsigned char key, int x, int y)
{
  if (ViewController::HasModel())
  {
    mModel->SpecialKeyboardChange(key, x, y);
  }
}

}  // namespace gloo.