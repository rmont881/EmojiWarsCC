//
//  GamePad.h
//  MyCppGame
//
//  Created by Josh Bodily on 10/10/16.
//
//

#pragma once
#define KEYBOARD_ONLY 1

#include "math/Vec2.h"

struct ControllerState {
  bool a, b, x, y, left_shoulder, right_shoulder, left_stick_press,
      right_stick_press, up, down, left, right, start, back;
  cocos2d::Vec2 left_stick, right_stick;
  unsigned short left_trigger, right_trigger;
  int player;
  void * controller;
};

namespace Util {
#if KEYBOARD_ONLY
ControllerState &getControllerState(int index);
#else
const ControllerState &getControllerState(size_t index);
#endif
void setListenForInput(bool listen);
}

enum Buttons {
  A = 1,
  B,
  X,
  Y,
  LB,
  RB,
  L_STICK,
  R_STICK,
  START,
  BACK,
  DPAD_UP = 12,
  DPAD_DOWN = 13,
  DPAD_LEFT = 14,
  DPAD_RIGHT = 15,
  // GenericDesktop Page
  LSTICK_X = 48,     // kHIDUsage_GD_X = 0x30
  LSTICK_Y = 49,     // kHIDUsage_GD_Y = 0x31
  TRIGGER_LEFT = 50, // kHIDUsage_GD_Z = 0x32
  RSTICK_X = 51,     // kHIDUsage_GD_Rx 0x33
  RSTICK_Y = 52,     // kHIDUsage_GD_Rx 0x33
  TRIGGER_RIGHT = 53 // kHIDUsage_GD_Rz = 0x35
};

void setupGamepad();
