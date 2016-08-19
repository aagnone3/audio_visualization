/**
 * Handles interaction with OpenGL and displaying additional content on the GUI canvas.
 * @author Anthony Agnone, Aug 2016
 */

#ifndef OPENGL_SPECTROGRAM_DISPLAY_H
#define OPENGL_SPECTROGRAM_DISPLAY_H

#include <vector>
#include <algorithm>
#include <GL/glut.h>
#include "AudioInput.hpp"
#include "GraphicsItem.hpp"

/* forward declarations */
class SpectrogramVisualizer;

class Display {
public:
  /**
   * Overloaded constructor to initialize various member parameters.
   */
  Display(int, char**, int);
  
  /**
   * De-allocates all dynamic memory.
   */
  ~Display();

  /**
   * Main GUI loop.
   */
  void loop();

  /**
   * Convenience function to draw larger text.
   * @param x coordinate x of the text.
   * @param y coordinate y of the text.
   * @param string text to draw.
   */
  static void largeText(float x, float y, char* string);

  /**
   * Convenience function to draw smaller text.
   * @param x coordinate x of the text.
   * @param y coordinate y of the text.
   * @param string text to draw.
   */
  static void smallText(float x, float y, char* string);

  /**
   * Loops through all observers and instructs them to display their content.
   * Follows the observer design pattern.
   */
  static void display();

  /**
   * Called by OpenGL during an idling period, and calls idle() on all of its observers.
   * Follows the observer design pattern.
   */
  static void idle(void);

  /**
   * Handles keyboard input from the user.
   * @param key identification character for the key pressed.
   * @param xPos coordinate x of the mouse when the key was pressed.
   * @param yPos coordinate y of the mouse when the key was pressed.
   */
  static void keyboard(unsigned char key, int xPos, int yPos);

  /**
   * Handles special keyboard input from the user.
   * @param key identification character for the key pressed.
   * @param xPos coordinate x of the mouse when the key was pressed.
   * @param yPos coordinate y of the mouse when the key was pressed.
   */
  static void special(int key, int xPos, int yPos);

  /**
   * Responds to a reshaping of the main window, reshaping all of its constituent plots and text.
   * @param w new width of the main window.
   * @param h new height of the main window.
   */
  static void reshape(int w, int h);

  /**
   * Responds to mouse input by the user.
   * @param button mouse button pressed by the user.
   * @param state whether the mouse button was just pressed or released by the user.
   * @param x coordinate x of the mouse cursor when the press happened.
   * @param y coordinate y of the mouse cursor when the press happened.
   */
  static void mouse(int button, int state, int x, int y);

  /**
   * Responds to mouse movement while one or more mouse buttons are pressed.
   * @param x coordinate x of the mouse cursor.
   * @param y coordinate y of the mouse cursor.
   */
  static void motion(int x, int y);

  /**
   * Adds a new GraphicsItem instance to the list of observers.
   * Follows the observer design pattern.
   * @param newItem new instance of GraphicsItem to act as an observer.
   */
  void addGraphicsItem(GraphicsItem* const newItem);

private:
  /**
   * A list of GraphicsItem instances to notify on each OpenGL callback, implementing the Observer design pattern.
   */
  static std::vector<GraphicsItem*> graphicsItems;

  /**
   * Title string for the GUI window.
   */
  static const char* const TITLE;

  /**
   * Indication of windowed or full screen mode for the GUI.
   *    0 -> windowed
   *    1 -> full screen
   */
  int screenMode;
};

#endif /* OPENGL_SPECTROGRAM_DISPLAY_H */
