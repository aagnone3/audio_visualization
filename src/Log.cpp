#include "Log.hpp"

/* define static members */
std::ofstream Log::file("log/program_log.txt");
Log* Log::instance;
unsigned int Log::OUTPUT_DIRECTION;

Log::~Log()
{
  file.close();
  delete instance;
}

void Log::log(std::string& message)
{
  switch (OUTPUT_DIRECTION) {
  case 0:
    std::cout << message << std::endl;
    break;
  case 1:
    if (!file) {
      throw "Log file not initialized!";
    }
    file << message << std::endl;
    break;
  case 2:
    std::cout << message << std::endl;
    if (!file) {
      throw "Log file not initialized!";
    }
    file << message << std::endl;
    break;
  default:
    break;
  }
}

void Log::log(const char* const message)
{
  switch (OUTPUT_DIRECTION) {
  case 0:
    std::cout << message << std::endl;
    break;
  case 1:
    if (!file) {
      throw "Log file not initialized!";
    }
    file << message << std::endl;
    break;
  case 2:
    std::cout << message << std::endl;
    if (!file) {
      throw "Log file not initialized!";
    }
    file << message << std::endl;
    break;
  default:
    break;
  }
}

std::ostream& Log::logger() {
  switch (OUTPUT_DIRECTION) {
  case 0:
    return std::cout;
  case 1:
    if (!file) {
      throw "Log file not initialized!";
    }
    return file;
  default:
    return std::cout;
  }
}

Log* Log::getInstance()
{
  if (!instance) {
    Log::instance = new Log();
  }
  return Log::instance;
}
