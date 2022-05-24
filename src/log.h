#ifndef LOG_H
#define LOG_H

#include <Arduino.h>

#ifdef LOG_LEVEL
  void log(const char *prefix, const char *file, const int line, const char *msg);
  void log_init(Print *printer);

  #define LOG_INIT(printer) do { log_init(printer); } while (0)
#endif

#if LOG_LEVEL > 0
  #define LOG_ERROR(msg)    do { log("E", __FILE__, __LINE__, msg); } while (0)
#else
  #define LOG_ERROR(msg)
#endif

#if LOG_LEVEL > 1
  #define LOG_INFO(msg)     do { log("I", __FILE__, __LINE__, msg); } while (0)
#else
  #define LOG_INFO(msg)
#endif

#if LOG_LEVEL > 2
  #define LOG_VERBOSE(msg)  do { log("V", __FILE__, __LINE__, msg); } while (0)
#else
  #define LOG_VERBOSE(msg)
#endif

#if LOG_LEVEL > 3
  #define LOG_DEBUG(msg)    do { log("D", __FILE__, __LINE__, msg); } while (0)
#else
  #define LOG_DEBUG(msg)
#endif

#endif
