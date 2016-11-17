//
//  GamePad.m
//  MyCppGame
//
//  Created by Josh Bodily on 10/10/16.
//
//

#import "GamePad.h"
#import <IOKit/hid/IOHIDLib.h>

IOHIDManagerRef hidManager;

std::map<void *, ControllerState> controllerStates;
std::map<int, ControllerState> playerStates;
bool listenForInput = false;

namespace Util {
#if KEYBOARD_ONLY
ControllerState &getControllerState(int index) {
  return playerStates[index];
  return controllerStates.begin()->second;
}
#else
const ControllerState &getControllerState(size_t index) {
  for(auto iterator = controllerStates.begin(); iterator != controllerStates.end(); iterator++) {
    if(iterator->second.player == index) {
      return iterator->second;
    }
  }
  return playerStates[index];
  return controllerStates.begin()->second;
}
#endif
void setListenForInput(bool listen) {
  listenForInput = listen;
}
}

void initGamepads() {
  playerStates[0];
  playerStates[0].player = 1;
  playerStates[1];
  playerStates[1].player = 2;
  playerStates[2];
  playerStates[2].player = 3;
  playerStates[3];
  playerStates[3].player = 4;
}

void initGamepad(struct ControllerState &state) {
  state.a = false;
  state.b = false;
  state.x = false;
  state.y = false;
  state.left_shoulder = false;
  state.right_shoulder = false;
  state.left_stick_press = false;
  state.right_stick_press = false;
  state.start = false;
  state.back = false;
  state.up = false;
  state.down = false;
  state.left = false;
  state.left_stick = cocos2d::Vec2::ZERO;
  state.right_stick = cocos2d::Vec2::ZERO;
  state.left_trigger = 0;
  state.right_trigger = 0;
  state.player = -1;
  state.controller = nullptr;
}

static CFMutableDictionaryRef
hu_CreateDeviceMatchingDictionary(UInt32 inUsagePage, UInt32 inUsage) {
  // create a dictionary to add usage page/usages to
  CFMutableDictionaryRef result = CFDictionaryCreateMutable(
      kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks,
      &kCFTypeDictionaryValueCallBacks);
  if (result) {
    if (inUsagePage) {
      // Add key for device type to refine the matching dictionary.
      CFNumberRef pageCFNumberRef =
          CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &inUsagePage);
      if (pageCFNumberRef) {
        CFDictionarySetValue(result, CFSTR(kIOHIDDeviceUsagePageKey),
                             pageCFNumberRef);
        CFRelease(pageCFNumberRef);

        // note: the usage is only valid if the usage page is also defined
        if (inUsage) {
          CFNumberRef usageCFNumberRef =
              CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &inUsage);
          if (usageCFNumberRef) {
            CFDictionarySetValue(result, CFSTR(kIOHIDDeviceUsageKey),
                                 usageCFNumberRef);
            CFRelease(usageCFNumberRef);
          } else {
            fprintf(stderr, "%s: CFNumberCreate(usage) failed.",
                    __PRETTY_FUNCTION__);
          }
        }
      } else {
        fprintf(stderr, "%s: CFNumberCreate(usage page) failed.",
                __PRETTY_FUNCTION__);
      }
    }
  } else {
    fprintf(stderr, "%s: CFDictionaryCreateMutable failed.",
            __PRETTY_FUNCTION__);
  }
  return result;
}

void gamepadWasAdded(void *inContext, IOReturn inResult, void *inSender,
                     IOHIDDeviceRef device) {
  NSLog(@"Gamepad was plugged in");
  if(playerStates.size() == 0) {
    initGamepads();
  }
//  controllerStates[device];
//  initGamepad(controllerStates[device]);
}

void gamepadWasRemoved(void *inContext, IOReturn inResult, void *inSender,
                       IOHIDDeviceRef device) {
  NSLog(@"Gamepad was unplugged");
  for(auto iterator = playerStates.begin(); iterator != playerStates.end(); iterator++) {
    if(iterator->second.controller == inSender) {
      iterator->second.controller = nullptr;
      break;
    }
  }
}

void gamepadAction(void *inContext, IOReturn inResult, void *inSender,
                   IOHIDValueRef value) {
    
  IOHIDElementRef element = IOHIDValueGetElement(value);
  uint32_t usagePage = IOHIDElementGetUsagePage(element);
  uint32_t usage = IOHIDElementGetUsage(element);
  int state = IOHIDValueGetIntegerValue(value);

  ControllerState* controller = nullptr;
  for(auto iterator = playerStates.begin(); iterator != playerStates.end(); iterator++) {
    if(iterator->second.controller == inSender) {
      controller = &iterator->second;
      break;
    }
  }
  
  if(listenForInput) {
    if (usagePage == kHIDPage_Button && usage == A) {
      bool alreadySet = false;
      for(int i = 0;i<4;i++) {
        if(playerStates[i].controller == inSender) {
          alreadySet = true;
          break;
        }
        if(!alreadySet && playerStates[i].controller == nullptr) {
          playerStates[i].controller = inSender;
          break;
        }
      }
    }
  }

  if(controller != nullptr) {
    if (usagePage == kHIDPage_Button) {
      if (usage == A)
        controller->a = state;
      else if (usage == B)
        controller->b = state;
      else if (usage == X)
        controller->x = state;
      else if (usage == Y)
        controller->y = state;
      else if (usage == LB)
        controller->left_shoulder = state;
      else if (usage == RB)
        controller->right_shoulder = state;
      else if (usage == L_STICK)
        controller->left_stick_press = state;
      else if (usage == R_STICK)
        controller->right_stick_press = state;
      else if (usage == START)
        controller->start = state;
      else if (usage == BACK)
        controller->back = state;
      else if (usage == DPAD_UP)
        controller->up = state;
      else if (usage == DPAD_DOWN)
        controller->down = state;
      else if (usage == DPAD_LEFT)
        controller->left = state;
      else if (usage == DPAD_RIGHT)
        controller->right = state;
    }
    if (usagePage == kHIDPage_GenericDesktop) {
      if (usage == LSTICK_X)
        controller->left_stick.x = state;
      else if (usage == LSTICK_Y)
        controller->left_stick.y = state;
      else if (usage == RSTICK_X)
        controller->right_stick.x = state;
      else if (usage == RSTICK_Y)
        controller->right_stick.y = state;
      else if (usage == TRIGGER_LEFT)
        controller->left_trigger = state;
      else if (usage == RSTICK_Y)
        controller->right_trigger = state;
    }
  }
}

void setupGamepad() {
  hidManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);

  auto matchingCFArrayRef =
      CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
  if (matchingCFArrayRef) {
    // create a device matching dictionary for joysticks
    CFDictionaryRef matchingCFDictRef = hu_CreateDeviceMatchingDictionary(
        kHIDPage_GenericDesktop, kHIDUsage_GD_Joystick);

    // create a device matching dictionary for game pads
    matchingCFDictRef = hu_CreateDeviceMatchingDictionary(
        kHIDPage_GenericDesktop, kHIDUsage_GD_GamePad);
    if (matchingCFDictRef) {
      // add it to the matching array
      CFArrayAppendValue(matchingCFArrayRef, matchingCFDictRef);
      CFRelease(matchingCFDictRef); // and release it
    } else {
      fprintf(stderr, "%s: hu_CreateDeviceMatchingDictionary(game pad) failed.",
              __PRETTY_FUNCTION__);
    }
  } else {
    fprintf(stderr, "%s: CFArrayCreateMutable failed.", __PRETTY_FUNCTION__);
  }
  auto matchingCFDictRef = CFDictionaryCreateMutable(
      kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks,
      &kCFTypeDictionaryValueCallBacks);
  // add the matching array to it
  CFDictionarySetValue(matchingCFDictRef, CFSTR(kIOHIDDeviceUsagePairsKey),
                       matchingCFArrayRef);
  // release the matching array
  CFRelease(matchingCFArrayRef);
  // set the HID device matching dictionary
  IOHIDManagerSetDeviceMatching(hidManager, matchingCFDictRef);
  // and then release it
  CFRelease(matchingCFDictRef);

  IOHIDManagerRegisterDeviceMatchingCallback(hidManager, gamepadWasAdded, NULL);
  IOHIDManagerRegisterDeviceRemovalCallback(hidManager, gamepadWasRemoved,
                                            NULL);
  IOHIDManagerScheduleWithRunLoop(hidManager, CFRunLoopGetCurrent(),
                                  kCFRunLoopDefaultMode);
  IOReturn tIOReturn = IOHIDManagerOpen(hidManager, kIOHIDOptionsTypeNone);
  IOHIDManagerRegisterInputValueCallback(hidManager, gamepadAction, NULL);
}
