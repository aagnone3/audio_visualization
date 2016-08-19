/*
 * Abstract representation of a class with performs some sort of action on an OpenGL graphics object. The Display class
 * maintains a list of GraphicsItem as observers which are notified upon each OpenGL callback to a static member
 * function of the subject Display class.
 * */

#ifndef OPENGL_SPECTROGRAM_GRAPHICSITEM_H
#define OPENGL_SPECTROGRAM_GRAPHICSITEM_H

class GraphicsItem {
public:
  virtual void display() = 0;

  virtual void displayText() = 0;

  virtual void idle() = 0;

  virtual void pause() = 0;

  /* keyboard key handler */
  virtual void keyboard(unsigned char, int, int) = 0;

  /* special keyboard key handler */
  virtual void special(int, int, int) = 0;

  virtual void reshape(int, int) = 0;

  virtual void mouse(int, int, int, int) = 0;

  virtual void motion(int, int) = 0;
protected:
  bool isPaused;
};

#endif //OPENGL_SPECTROGRAM_GRAPHICSITEM_H
