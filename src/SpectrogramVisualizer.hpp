/**
 * Obtains and draws a spectrogram from continuous audio input.
 * Implements methods from the abstract class GraphicsItem, which are called by a subject Display instance as an
 * implementation of the observer design pattern.
 * @author Anthony Agnone, Aug 2016
 */

#ifndef OPENGL_SPECTROGRAM_SCENE_H
#define OPENGL_SPECTROGRAM_SCENE_H

#include <GLUT/glut.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>
#include <fftw3.h>
#include "AudioInput.hpp"
#include "PortAudio.hpp"
#include "Display.hpp"
#include "GraphicsItem.hpp"
#include "shared.hpp"

class SpectrogramVisualizer : public GraphicsItem {
public:
    /**
     * Simple array of canonical note names e.g. C#, Cb, C, etc.
     * */
    static const char *const noteNames[];
    /**
     * Struct with multiple char's representing specific keyboard keys corresponding to desired program actions.
     */
    struct shortcuts {
        char ESCAPE;
        char QUIT;
        char DIAGNOSE;
        char PAUSE;
        char SAMPLE_RATE_UP;
        char SAMPLE_RATE_DOWN;
        char CHANGE_COLOR_SCHEME;
    };

    /**
     * Specific implementation of the shortcuts struct for this particular visualizer.
     */
    static const shortcuts KEYBOARD_SHORTCUTS;

    /**
     * Canonical middle C note frequency.
     * https://en.wikipedia.org/wiki/C_(musical_note)#Middle_C
     */
    static const float MIDDLE_C_FREQUENCY;

    /**
     * Canonical number of semitones per octave.
     * https://en.wikipedia.org/wiki/Equal_temperament
     */
    static const unsigned int N_SEMITONES_PER_OCTAVE;

    /**
     * Overloaded constructor to initialize various member parameters.
     */
    SpectrogramVisualizer(int);

    /**
     * Empty destructor.
     */
    ~SpectrogramVisualizer();

    /**
     * Displays all information and graphics on the screen.
     * This function is routinely called by the Display subject, and is one of many functions that the class defines as
     * a GraphicsItem observer in the observer design pattern.
     */
    virtual void display();

    /**
     * Displays all text on the canvas.
     */
    virtual void displayText();

    /**
     * Called during an idling period.
     */
    virtual void idle();

    /**
     * Called during a pause period.
     */
    virtual void pause();

    /**
     * Responds to a pressed keyboard key to provide an appropriate change in display or functionality, according to
     * SpectrogramVisualizer::KEYBOARD_SHORTCUTS.
     * @param key char representation of the key pressed.
     * @param xPos mouse x position when the key was pressed.
     * @param yPos mouse y position when the key was pressed.
     */
    virtual void keyboard(unsigned char key, int xPos, int yPos);

    /**
     * Responds to a pressed special keyboard key to provide an appropriate change in display or functionality,
     * according to
     * SpectrogramVisualizer::KEYBOARD_SHORTCUTS.
     * @param key char representation of the special key pressed.
     * @param xPos mouse x position when the key was pressed.
     * @param yPos mouse y position when the key was pressed.
     */
    virtual void special(int key, int xPos, int yPos);

    /**
     * Responds to a reshaping of the main window, reshaping all of its constituent plots and text.
     * @param w new width of the main window.
     * @param h new height of the main window.
     */
    virtual void reshape(int w, int h);

    /**
     * Responds to mouse input by the user.
     * @param button mouse button pressed by the user.
     * @param state whether the mouse button was just pressed or released by the user.
     * @param x coordinate x of the mouse cursor when the press happened.
     * @param y coordinate y of the mouse cursor when the press happened.
     */
    virtual void mouse(int button, int state, int x, int y);

    /**
     * Responds to mouse movement while one or more mouse buttons are pressed.
     * @param x coordinate x of the mouse cursor.
     * @param y coordinate y of the mouse cursor.
     */
    virtual void motion(int x, int y);

private:
    /**
     * Flag to show medical info.
     */
    bool diagnose;
    /**
     * Buffer for displaying diagnosis info.
     */
    char diagnosis[1000];
    /**
     * GL window width and height.
     */
    float viewportSize[2];
    /**
     * Spectrogram image log color mapping parameters.
     */
    float colorScale[2];
    /**
     * Handle for mouse activity on the spectrogram.
     */
    float mouseHandle[3];
    /**
     * Maintains length of current run time.
     */
    float runTime;
    /**
     * Frames per second counter.
     */
    int frameCount;
    /**
     * Frames per second indicator.
     */
    int fps;

    /**
     * Frames per second tick reference.
     */
    time_t fpsTick;
    /**
     * Holds the start time of the program for runtime information.
     */
    timeval startTime;
    /**
     * Toggles frequency read-off lines on the spectrogram display.
     * Range of values is [0,2].
     * 0 -> no read-off lines
     * 1 -> single read-off line where mouse was clicked
     * 2 -> read-off lines for where mouse was clicked and harmonics of that frequency
     */
    int frequencyReadOff;
    /**
     * Controls speed of spectrogram scrolling. When scrollCount reaches scrollFactor, the spectrogram display
     * is scrolled. Therefore, a smaller value leads to faster scrolling.
     */
    int scrollFactor;
    /**
     * Continuous counter from [0, scollFactor]. When scrollCount reaches scrollFactor, the spectrogram display
     * is scrolled.
     */
    int scrollCount;
    /**
     * Current mode of the spectrogram's color display.
     * Range of values if [0,2].
     * 0 -> black background with white spectral information
     * 1 -> white background with black spectral information
     * 2 -> black background with RGB spectral information
     */
    int colorMode;
    /**
     * Obtains continous audio input from the microphone or an alternative audio source.
     */
    AudioInput *audioInput;
    /**
     * An array of bytes, each representing a color mapped from the float value of a spectrogram.
     */
    char *spectrogramBytes;
    /**
     * A float array of spectrogram values.
     */
    float *spectrogramFloat;
    /**
     * Hz per pixel for plotting a spectral x-axis.
     */
    float hzPerPixelX;
    /**
     * Hz per pixel for plotting a spectral y-axis.
     */
    float hzPerPixelY;
    /**
     * Highest frequency that the spectrogram will display.
     */
    float highestFrequency;

    /**
     * Displays the time domain representation of the signal.
     */
    void plotTimeDomain();

    /**
     * Displays the log magnitude spectral domain representation of the signal.
     */
    void plotSpectralMagnitude();

    /**
     * Displays the spectrogram representation of the signal.
     */
    void plotSpectrogram();

    /**
     * Draws the various plot axes onto the display.
     * @param xStart starting coordinate for x axis
     * @param xEnd ending coordinate for x axis
     * @param yStart starting coordinate for y axis
     * @param yEnd ending coordinate for y axis
     * @param xFudgeFactor fudge factor for tick marks on x axis
     * @param yFudgeFactor fudge factor for tick marks on y axis
     * @param xLabel label for x axis
     * @param yLabel label for y axis
     */
    void drawAxes(float xStart, float xEnd, float yStart, float yEnd, float xFudgeFactor, float yFudgeFactor,
                  char *xLabel, char *yLabel);

    /**
     * Converts a float spectrogram value to its 8-bit color char representation.
     * @author: Alex Barnett 01/25/2015.
     * @param x float spectrogram value.
     * @return 8-bit color char representation of x.
     */
    char colorByteMap(float x);

    /**
     * Converts the entire spectrogram array from float values to 8-bit color char values
     * using SpectrogramVisualizer::colorByteMap().
     */
    void recomputeSpectrogramBytes();

    /**
     * Computes the number of tick marks for the x-axis of the spectrogram.
     * Returns the zero-indexed locations of the tick marks via the tics parameter.
     * This is a single-precision version of Alex Barnett's ~visu/viewer/viewer.c, but with a density fudge factor.
     * A fudge factor of 0 leads to simple default values.
     * @author: Alex Barnett
     *
     * @param lowValue lowest tick value to start with
     * @param range total range that the ticks should encompass
     * @param fudgeFactor fudge factor for tick density
     * @param tickMarks pointer to float array of tick marks created
     * @return number of tick marks created
     */
    static int chooseTics(float lowValue, float range, float fudgeFactor, float *tickMarks);

    /**
     * Scrolls the entire spectrogram display over to display the signal's spectral information over time.
     */
    void scrollSpectrogram();
};

#endif //OPENGL_SPECTROGRAM_SCENE_H
