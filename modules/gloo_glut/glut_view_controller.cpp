#include "glut_view_controller.h"
#include "model_base.h"

namespace gloo
{
 
GlutViewController::GlutViewController()
{

}

GlutViewController::~GlutViewController()
{
  if (HasModel() && OwnsModel())
  {
    delete mModel;
  }
}

bool GlutViewController::Init()
{
  if (HasModel())
  {
    return mModel->Init();
  }

  return false;
}

void GlutViewController::Idle()
{
  if (HasModel())
  {
    mModel->Idle();
  }
}

void GlutViewController::Display()
{
  if (HasModel())
  {
    mModel->Display();
  }
}

void GlutViewController::Reshape(int w, int h)
{
  if (HasModel())
  {
    mModel->Reshape(w, h);
  }
}

void GlutViewController::ActiveMouseMotion(int x, int y)
{
  if (HasModel())
  {
    mModel->ActiveMouseMotion(x, y);
  }
}

void GlutViewController::PassiveMouseMotion(int x, int y)
{
  if (HasModel())
  {
    mModel->PassiveMouseMotion(x, y);
  }
}

void GlutViewController::MouseButtonChange(int button, int state, int x, int y)
{
  if (HasModel())
  {
    mModel->MouseButtonChange(button, state, x, y);
  }
}

void GlutViewController::KeyboardChange(unsigned char key, int x, int y)
{
  if (HasModel())
  {
    mModel->KeyboardChange(key, x, y);
  }
}

void GlutViewController::SpecialKeyboardChange(unsigned char key, int x, int y)
{
  if (HasModel())
  {
    mModel->SpecialKeyboardChange(key, x, y);
  }
}

}  // namespace gloo.