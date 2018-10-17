#include <algorithm>
#include "common.h"
#include "AudioInput.hpp"
#include "Display.hpp"
#include "GraphicsItem.hpp"
#include "SpectrogramVisualizer.hpp"
#include "AudioVisualizationConfig.h"
#include "Log.hpp"

int screenMode;
unsigned int verbosity;
int scrollFactor;

const char* const helptext[] = {
    "Real Time Audio Visualization\n",
    "Author: Anthony Agnone\n\n",
    "Usage: audio_visualization [-f] [-v] [-V] [-sf <scroll_factor>] [-w <windowType>]\n\n",
    "\t[-f] enables full-screen-mode\n",
    "\t[-v] print version and exit\n",
    "\t[-V] set verbosity int\n",
    "\t[-w] short-time window function type, default: 2\n",
              "\t\t0: no window (or equivalently a rectangular window\n",
              "\t\t1: Hann window\n",
              "\t\t2: Gaussian truncated at +-4sigma)\n",
    "\t\t[-sf] scroll_factor = 1,2,... # vSyncs (60Hz) to wait per scroll pixel (default 1)\n\n",
    "Keys & Mouse Controls\n",
    "\t\tarrows or middle button drag - brightness/contrast\n",
    "\t\tleft button shows horizontal frequency readoff line\n",
    "\t\tright button shows horizontal frequency readoff with multiples\n",
    "\t\ti - cycles through color maps (B/W, inverse B/W, color)\n",
    "\t\tq or Esc - quit\n",
    "\t\t[ and ] - control horizontal scroll factor (samplingRate)\n"
};


int getInputDeviceId(const char *fn)
{
    //YAML::Node config = YAML::LoadFile(fn);
    //return config["device_id"].as<int>();
    return 2;
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
    else if (!strcmp(argv[i], "-V")) {
      verbosity = 1;
    }
    else if (!strcmp(argv[i], "-v")) {
      fprintf(stdout, "Version: %d.%d\n\n", AudioVisualization_VERSION_MAJOR, AudioVisualization_VERSION_MINOR);
      exit(1);
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
