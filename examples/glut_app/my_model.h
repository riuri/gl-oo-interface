#include <gloo/model_base.h>

using namespace gloo;

class MyModel : public ModelBase
{
public:
  MyModel();
  virtual ~MyModel() {}

  virtual bool Init();

  // -- Essential Callbacks --
  virtual void Idle();
  virtual void Display();
  virtual void Reshape(int w, int h);

  // -- Mouse Callbacks --
  virtual void  ActiveMouseMotion(const MouseEvent & mouseEvent);
  virtual void PassiveMouseMotion(const MouseEvent & mouseEvent);
  virtual void  MouseButtonChange(const MouseEvent & mouseEvent);

  // -- Keyboard Callbacks --
  virtual void KeyboardChange(unsigned char key, int x, int y);
  virtual void SpecialKeyboardChange(unsigned char key, int x, int y);

private:

};