#pragma once

#include <inttypes.h>

enum DataSrc { SWITCH, BUTTON_L, BUTTON_R, RHEOSTAT, JOYSTICK };

struct Msg {
  enum DataSrc src;
  uint32_t payload;
};

void analogue_read_t();
void setup_buttons();
void buttons_read_t();
