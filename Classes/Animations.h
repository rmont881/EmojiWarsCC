//
//  Animations.h
//  MyCppGame
//
//  Created by Josh Bodily on 11/6/16.
//
//

#pragma once

namespace cocos2d { class Animate; class Sprite; }

class Animations {
public:
    static void loadAnimations(const std::string& file);
    static void runAnimation(cocos2d::Sprite* target, const std::string& animation, bool forceRestart = false);
private:
    static std::map<std::string, cocos2d::Animate*> _animations;
    static std::map<std::string, int> _tags;
};