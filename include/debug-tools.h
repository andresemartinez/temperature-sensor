#ifndef DEBUG_TOOLS_H
#define DEBUG_TOOLS_H

#include <config.h>

#ifdef DEBUG

#define initDebugLog(x) Serial.begin(x);
#define debugLog(x) Serial.print(x);
#define debugLogLn(x) Serial.println(x);
#define debugDo(x) x;

#else

#define initDebugLog(x) x;
#define debugLog(x) x;
#define debugLogLn(x) x;
#define debugDo(x);

#endif

#endif
