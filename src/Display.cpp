//
// Created by aagnone3 on 8/15/16.
//

#include "Display.hpp"

/* static member initializations */
const char* const Display::TITLE = "OpenGL Spectrum Visualization by Anthony Agnone, Aug 2016";
std::vector<GraphicsItem*> Display::graphicsItems;

Display::Display(int argc, char** argv, int screenMode)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  if (screenMode==1) {  // implement desired screen mode
    glutGameModeString("1024x768:24@60");  /* res, pixel depth, refresh */
    glutEnterGameMode();  /* starts fullscreen game mode */
  }
  else {
    glutInitWindowSize(1024, 768);  // window same bufferSizeFrames as XGA
    int mainWindow = glutCreateWindow(TITLE);
    //  glutFullScreen();    // maximizes window, but is not game mode
  }

  glutDisplayFunc(Display::display);
  glutReshapeFunc(Display::reshape);
  glutKeyboardFunc(Display::keyboard);
  glutSpecialFunc(Display::special);
  glutMouseFunc(Display::mouse);
  glutMotionFunc(Display::motion);
  glutIdleFunc(Display::idle);

  this->screenMode = screenMode;
}

Display::~Display()
{
  delete TITLE;
}

void Display::loop()
{
  /* pass control to GLUT */
  glutMainLoop();
}

void Display::largeText(float x, float y, char* string)
{
  int len, i;
  glRasterPos2f(x, y);
  len = (int) strlen(string);
  for (i = 0; i<len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
  }
}

void Display::smallText(float x, float y, char* string)
{
  int len, i;
  glRasterPos2f(x, y);
  len = (int) strlen(string);
  for (i = 0; i<len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, string[i]);
  }
}

void Display::display()
{
  glClear(GL_COLOR_BUFFER_BIT); // no depth buffer
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 1, 0, 1, -1, 1);  // l r b t n f
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  std::for_each(graphicsItems.begin(), graphicsItems.end(), [&](auto item) { item->display(); });
  // glFinish();   // wait for all gl commands to complete

  glutSwapBuffers(); // for this to WAIT for vSync, need enable in NVIDIA OpenGL
}

void Display::idle(void)
{
  std::for_each(graphicsItems.begin(), graphicsItems.end(), [&](auto item) { item->idle(); });
  glutPostRedisplay();  /* trigger GLUT display function */
}

void Display::keyboard(unsigned char key, int xPos, int yPos)
{
  std::for_each(graphicsItems.begin(), graphicsItems.end(), [&](auto item) { item->keyboard(key, xPos, yPos); });
}

void Display::special(int key, int xPos, int yPos)
{
  std::for_each(graphicsItems.begin(), graphicsItems.end(), [&](auto item) { item->special(key, xPos, yPos); });
}

void Display::reshape(int w, int h)
{
  Log::getInstance()->logger() << "Setting w=" << w << ", h=" << h << std::endl;
  std::for_each(graphicsItems.begin(), graphicsItems.end(), [&](auto item) { item->reshape(w, h); });
  glViewport(0, 0, w, h);
}

void Display::mouse(int button, int state, int x, int y)
{
  std::for_each(graphicsItems.begin(), graphicsItems.end(), [&](auto item) { item->mouse(button, state, x, y); });
}

void Display::motion(int x, int y)
{
  std::for_each(graphicsItems.begin(), graphicsItems.end(), [&](auto item) { item->motion(x, y); });
}

void Display::addGraphicsItem(GraphicsItem* const newItem)
{
  graphicsItems.push_back(newItem);
}
