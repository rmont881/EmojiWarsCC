//
//  Constants.h
//  MyCppGame
//
//  Created by Josh Bodily on 10/25/16.
//
//

#pragma once

#define DEBUG_DRAW          0

// Item tags
#define WEAPON_TAG          1

// Physics
#define FIXED_TIME_STEP     0.016667f
#define GRAVITY_Y           -2000.0f

// Collider flags
#define COLLIDE_ALL_SIDES   0xFF

// Screen
#define RESOLUTION_WIDTH    30 * 32
#define RESOLUTION_HEIGHT   20 * 32
#define SCREEN_CENTER       cocos2d::Vec2(RESOLUTION_WIDTH / 2, RESOLUTION_HEIGHT / 2)

// Strings
#define GAME_NAME           "Working Title"