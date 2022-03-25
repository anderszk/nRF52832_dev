#pragma once

#ifndef ANALOG_IN_H
#define ANALOG_IN_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

  float AnalogRead(int channel);
  #define BAD_ANALOG_READ -123

#ifdef __cplusplus
}
#endif


#endif
