#include "log.h"
Print *logPrinter = nullptr;

void log_init(Print *printer) {
  logPrinter = printer;
}

void log(const char *prefix, const char *file, const int line, const char *msg)
{
  logPrinter->print(prefix);
  logPrinter->print(" (");
  logPrinter->print(file);
  logPrinter->print(":");
  logPrinter->print(line);
  logPrinter->print("): ");
  logPrinter->println(msg);
}
