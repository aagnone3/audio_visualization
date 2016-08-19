/**
 * Singleton class to handle logging to standard output or a file.
 */

#ifndef OPENGL_SPECTROGRAM_LOG_H
#define OPENGL_SPECTROGRAM_LOG_H

#include <ostream>
#include <fstream>
#include <iostream>
#include <string>

class Log {
public:
  /**
   * De-allocates all dynamic memory.
   */
  ~Log();

  /**
   * Logging method for a string input.
   * @param message the message to log.
   */
  void log(std::string& message);

  /**
   * Logging method for a const char* const input.
   * @param message the message to log.
   */
  void log(const char* const message);

  /**
   * Returns the appropriate logger, according to Log::OUTPUT_DIRECTION. This is useful with the stream operator.
   * @return appropriate logger, according to Log::OUTPUT_DIRECTION. See its documentation for more details.
   */
  std::ostream& logger();

  /**
   * Accessor method for the singleton instance of the class. If the instance does not exist, then (and only then) a
   * new instance is created.
   * @return
   */
  static Log *getInstance();
private:
  /**
   * Direction of output for logging/debugging.
   *    0 -> std::cout
   *    1 -> file with low verbosity
   *    2 -> file with high verbosity
   */
  static unsigned int OUTPUT_DIRECTION;

  /**
   * File handle for output, if applicable.
   */
  static std::ofstream file;

  /**
   * Private Log instance pointer to implement the singleton design pattern.
   */
  static Log *instance;
};

#endif /* OPENGL_SPECTROGRAM_LOG_H */
