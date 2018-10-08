// glSpect - OpenGL real-time spectrogram
// Alex Barnett 12/21/10, based on Luke Campagnola's nice 2005 glScope.
// ahb@math.dartmouth.edu
// Tweaked 10/13/11 for inverse video, etc.
// 1/25/14 fix -EPIPE snd_pcm_readi() error. nchannels=2, windowingFunction=0 case, etc
//        added colorByteMap to clean up color mapping; color map.
// 1/17/16: freq indicator line via left-button

/* Notes: be sure to set vSync wait in graphics card (eg NVIDIA) OpenGL settings
 */

#include <algorithm>
#include <yaml.h>
#include "AudioInput.hpp"
#include "Display.hpp"
#include "GraphicsItem.hpp"
#include "SpectrogramVisualizer.hpp"
#include "Log.hpp"

int screenMode;
unsigned int verbosity;
int scrollFactor;

const char* const helptext[] = {
    "glSpect: real-time OpenGL spectrogram.  Anthony Agnone\n",
    "Usage: glspect  [-f] [-v] [-sf <scroll_factor>] [-w <windowType>]\n\n",
    "Command line arguments:\n",
    "windowType = \t0 (no window)\n\t\t1 (Hann)\n\t\t2 (Gaussian trunc at +-4sigma) (default)\n",
    "scroll_factor = 1,2,... # vSyncs (60Hz) to wait per scroll pixel (default 1)\n",
    "Keys & mouseHandle: \tarrows or middle button drag - brightness/contrast\n",
    "\t\tleft button shows horizontal frequency readoff line\n",
    "\t\tright button shows horizontal frequency readoff with multiples\n",
    "\t\ti - cycles through color maps (B/W, inverse B/W, color)\n",
    "\t\tq or Esc - quit\n",
    "\t\t[ and ] - control horizontal scroll factor (samplingRate)\n",
    NULL};


int getInputDeviceId(const char *fn)
{
    YAML::Node config = YAML::LoadFile(fn);
    return config["device_id"].as<int>();
} 

int main(int argc, char** argv)
{
  /* set default values, and change as specified by the user via command line options */
  screenMode = 0;  /* default to windowed unless user specifies full via -f */
  verbosity = 0;  /* default to std::cout */
  scrollFactor = 2;  /* how many vSyncs to wait before scrolling spectrogram */

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
  try {
      SpectrogramVisualizer spectrogramVisualizer(scrollFactor, getInputDeviceId("cfg.yaml"));
      display.addGraphicsItem(&spectrogramVisualizer);
  } catch (int e) {
      std::cout << "Error starting the visualization. Exiting." << std::endl;
      exit(1);
  }

  display.loop();  /* main loop */
  return 0;
}
