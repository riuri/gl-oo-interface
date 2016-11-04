#include "glut_application.h"

#include <iostream>

#define LOG_OUTPUT_ON 1

namespace gloo
{

GlutViewController* GlutApplication::sViewController = nullptr;

// ===================== Static functions for OpenGL callbacks ==========================  
void GlutApplication::Idle()
{
  sViewController->Idle();
}

void GlutApplication::Display() 
{
  sViewController->Display();
}

void GlutApplication::Reshape(int w, int h)
{
  sViewController->Reshape(w, h);
}

void GlutApplication::ActiveMouseMotion(int x, int y)
{
  sViewController->ActiveMouseMotion(x, y);
}

void GlutApplication::PassiveMouseMotion(int x, int y)
{
  sViewController->PassiveMouseMotion(x, y);
}

void GlutApplication::MouseButtonChange(int button, int state, int x, int y)
{
  sViewController->MouseButtonChange(button, state, x, y);
}

void GlutApplication::KeyboardChange(unsigned char key, int x, int y)
{
  sViewController->KeyboardChange(key, x, y);
}

void GlutApplication::SpecialKeyboardChange(int key, int x, int y)
{
  sViewController->SpecialKeyboardChange(key, x, y);
}

// ======================================================================================  

int GlutApplication::Run(int argc, char* argv[], ModelBase* model,
                           const std::string & windowTitle,
                           int windowWidth, int windowHeight,
                           unsigned int displayMode)
{
#if LOG_OUTPUT_ON == 1
  std::cout << "------------------------------------------------------------------" << std::endl;
  std::cout << "1. Initializing GLUT..." << std::endl;
#endif

#ifdef __LINUX__
  GLenum err = glewInit();
  
  // Check if machine supports the 2.1 API.
  if (err != GLEW_OK || (!GLEW_VERSION_2_1))
  {
#if LOG_OUTPUT_ON == 1
    std::cerr << "ERROR: could not load GLEW on Linux. " << std::endl;
#endif

    return 1;
  }
#endif
  
  // Create context.
  glutInit(&argc, argv);

#if LOG_OUTPUT_ON == 1
  std::cout << "2. Initializing OpenGL..." << std::endl;
#endif

#ifdef __APPLE__
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | displayMode);
#else
    glutInitDisplayMode(displayMode);
#endif
  
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(windowWidth, windowHeight);
  glutCreateWindow(windowTitle.c_str());

#if LOG_OUTPUT_ON
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
#endif

  sViewController = new GlutViewController();

  if (sViewController == nullptr)
  {
#if LOG_OUTPUT_ON == 1
    std::cerr << "ERROR: Couldn't allocate GlutViewController. " << std::endl;
#endif
    return 1;
  }

  if (!model->Init())
  {
#if LOG_OUTPUT_ON == 1
    std::cerr << "ERROR: Couldn't initialize internal model"
                 " (GlutApplication::sViewController::mModel). " << std::endl;
#endif
    return 1;
  }  

#if LOG_OUTPUT_ON == 1
  std::cout << "Initialization Complete." << std::endl;
#endif

  sViewController->SetModel(model, true);

  // Install callbacks.
  glutDisplayFunc(GlutApplication::Display);
  glutIdleFunc(GlutApplication::Idle); 
  glutReshapeFunc(GlutApplication::Reshape); 
  glutMotionFunc(GlutApplication::ActiveMouseMotion);
  glutPassiveMotionFunc(GlutApplication::PassiveMouseMotion);
  glutMouseFunc(GlutApplication::MouseButtonChange);
  glutKeyboardFunc(GlutApplication::KeyboardChange);
  glutSpecialFunc(GlutApplication::SpecialKeyboardChange);

  glutMainLoop();

  delete sViewController;

  return 0;
}


}  // namespace gloo.