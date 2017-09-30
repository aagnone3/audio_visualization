// glSpect - OpenGL real-time spectrogram
// Alex Barnett 12/21/10, based on Luke Campagnola's nice 2005 glScope.
// ahb@math.dartmouth.edu
// Tweaked 10/13/11 for inverse video, etc.
// 1/25/14 fix -EPIPE snd_pcm_readi() error. nchannels=2, windowingFunction=0 case, etc
//        added colorByteMap to clean up color mapping; color map.
// 1/17/16: freq indicator line via left-button

/* Notes: be sure to set vSync wait in graphics card (eg NVIDIA) OpenGL settings
 */

/* ISSUES: (2011)
 * occasional dropped audio every few secs - why? (1470 vs 1472? issue)
 * jitter in signal plotTimeDomain scrolling
 * GlutGameMode isnt' setting refresh to 60Hz, rather 75Hz.
 * Better than glDrawPixels (which sends all data every frame to GPU) woudl be
    to pass the data as a texture and scroll it in the GPU (a convolution?),
    modifying only one row each time. This would be super low CPU usage!
 * Use GL_ARB_pixel_buffer_object for fast glDrawPixels or textures via DMA?
 * Add color?
 * add playback of audio file, or jack into audio playback?
 * glDrawpixels is deprecated in OpenGl >3.0. THat's annoying.
   Eg: https://www.opengl.org/discussion_boards/showthread.php/181907-drawing-individual-pixels-with-opengl
  "The modern way to do it is to store the data in a texture then draw a pair of textured triangles (quads are also deprecated)."
 */

/* SOLVED ISSUES: (2011)
 * Creation and initialization of global visualizer happens before main(), bad, since
     couldn't set bufferMemorySeconds in cmd line! Ans: use trivial creator/destructor,
     and call other init/close routines from main().
 */

#include <algorithm>
#include "AudioInput.hpp"
#include "Display.hpp"
#include "GraphicsItem.hpp"
#include "SpectrogramVisualizer.hpp"
#include "Log.hpp"

// Various parameters
struct Param {
  // DFT windowing function type
  int windowType;
  // Power of 2 giving DFT fftLength (N) in samples
  int windowSizeExponent;
};
Param param;
int screenMode;
unsigned int verbosity;
int scrollFactor;

const char* const helptext[] = {
    " glSpect: real-time OpenGL spectrogram.  Anthony Agnone\n",
    "Usage: glspect  [-f] [-v] [-sf <scroll_factor>] [-w <windowType>] [-t windowSizeExponent]\n\n",
    "Command line arguments:\n",
    "windowType = \t0 (no window)\n\t\t1 (Hann)\n\t\t2 (Gaussian trunc at +-4sigma) (default)\n",
    "scroll_factor = 1,2,... # vSyncs (60Hz) to wait per scroll pixel (default 1)\n",
    "windowSizeExponent = 11,12,...,16 is power of 2 giving FFT fftLength N (default 13)\n\t(Note: this controls the vertical frequency resolution and range)\n\n",
    "Keys & mouseHandle: \tarrows or middle button drag - brightness/contrast\n",
    "\t\tleft button shows horizontal frequency readoff line\n",
    "\t\tright button shows horizontal frequency readoff with multiples\n",
    "\t\ti - cycles through color maps (B/W, inverse B/W, color)\n",
    "\t\tq or Esc - quit\n",
    "\t\t[ and ] - control horizontal scroll factor (samplingRate)\n",
    NULL};


int main(int argc, char** argv)
{
  /* set default values, and change as specified by the user via command line options */
  screenMode = 0;  /* default to windowed unless user specifies full via -f */
  verbosity = 0;  /* default to std::cout */
  scrollFactor = 2;  /* how many vSyncs to wait before scrolling spectrogram */
  param.windowType = 2;  /* Gaussian window */
  param.windowSizeExponent = 13;  /* 8192 samples (around 0.19 sec). Remains fixed. */

  /* parse command line options from the user */
  for (int i = 1; i<argc; ++i) {
    if (!strcmp(argv[i], "-f")) {
      screenMode = 1;
    }
    else if (!strcmp(argv[i], "-v")) {
      verbosity = 1;
    }
    else if (!strcmp(argv[i], "-sf")) {
      sscanf(argv[++i], "%d", &scrollFactor);
      scrollFactor = std::min(scrollFactor, 1);  /* ensure value <= 1 */
    }
    else if (!strcmp(argv[i], "-t")) {
      sscanf(argv[++i], "%d", &param.windowSizeExponent);
      /* ensure range of [10,18] */
      param.windowSizeExponent = std::max(10, param.windowSizeExponent);
      param.windowSizeExponent = std::min(18, param.windowSizeExponent);
    }
    else if (!strcmp(argv[i], "-w")) {
      sscanf(argv[++i], "%d", &param.windowType);
      param.windowType = std::max(0, param.windowType);  /* ensure value >= 0 */
    }
    else {
      /* misuse or -h, print out usage text */
      fprintf(stderr, "bad command line option %s\n\n", argv[i]);
      for (int j = 0; helptext[j]; j++) {
        fprintf(stderr, "%s", helptext[j]);
      }
      std::cout << helptext << std::endl;
      exit(1);
    }
  }
  Log::OUTPUT_DIRECTION = verbosity;
  Display display(argc, argv, screenMode);

  /* create GraphicsItem observers and add them to the display's observer list */
  SpectrogramVisualizer spectrogramVisualizer(scrollFactor);
  display.addGraphicsItem(&spectrogramVisualizer);

  display.loop();  /* main loop */
  return 0;
}
