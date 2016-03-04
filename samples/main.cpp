/******************************************+
*                                          *
*  CSCI420 - Computer Graphics USC         *
*  Author: Rodrigo Castiel                 *
*                                          *
+*******************************************/

#include "roller_coaster_program.h"

RollerCoasterProgram* program = nullptr;

// GLUT Callbacks ---------------------------------------
void DisplayFunc() 
{
  program->DisplayFunc();
}

void IdleFunc()
{
  program->IdleFunc();
}

void ReshapeFunc(int w, int h)
{
  program->ReshapeFunc(w, h);
}

void MotionFunc(int x, int y)
{
  program->MotionFunc(x, y);
}

void PassiveMotionFunc(int x, int y)
{
  program->PassiveMotionFunc(x, y);
}

void MouseFunc(int button, int state, int x, int y)
{
  program->MouseFunc(button, state, x, y);
}

void KeyboardFunc(unsigned char key, int x, int y)
{
  program->KeyboardFunc(key, x, y);
}
// GLUT Callbacks END -----------------------------------

int main(int argc, char *argv[])
{
  program = new RollerCoasterProgram();
  program->Init(&argc, argv, "CSCI 420 Homework II - Roller Coaster Simulator");
  program->InitScene(argc, argv);

  // NOTE: Next section is based on starter code.
  glutDisplayFunc(DisplayFunc); // tells glut to use a particular display function to redraw 
  glutIdleFunc(IdleFunc);       // perform animation inside idleFunc
  glutReshapeFunc(ReshapeFunc); // callback for resizing the window
  glutMotionFunc(MotionFunc);   // callback for mouse drags
  glutPassiveMotionFunc(PassiveMotionFunc);  // callback for idle mouse movement
  glutMouseFunc(MouseFunc);        // callback for mouse button changes
  glutKeyboardFunc(KeyboardFunc);  // callback for pressing the keys on the keyboard
  // END.

  program->Run();
  delete program;

  return 0;
}

