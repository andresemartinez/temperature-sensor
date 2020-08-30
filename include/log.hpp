#ifndef LOG_H
#define LOG_H

#include <config.hpp>

#ifdef DEBUG

#define initLog(x) Serial.begin(x);
#define log(x) Serial.print(x);
#define logln(x) Serial.println(x);

#else

#define initLog(x) x;
#define log(x) x;
#define logln(x) x;

#endif

#endif
