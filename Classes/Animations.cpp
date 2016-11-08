//
//  Animations.cpp
//  MyCppGame
//
//  Created by Josh Bodily on 11/6/16.
//
//

#include "Animations.h"

std::map<std::string, cocos2d::Animate*> Animations::_animations;
std::map<std::string, int> Animations::_tags;

void Animations::loadAnimations(const std::string& file) {
    static int tag = 0;
    _animations.clear();
    _tags.clear();
    
    cocos2d::AnimationCache::getInstance()->addAnimationsWithFile(file);
    cocos2d::ValueMap data;
    std::string path = cocos2d::FileUtils::getInstance()->fullPathForFilename(file);
    data = cocos2d::FileUtils::getInstance()->getValueMapFromFile(path);
    auto arrAnimations = data.at("animations").asValueMap();
    
    for (const auto& pair : arrAnimations) {
        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(pair.first);
        auto action = cocos2d::Animate::create(animation);
        action->retain();
        action->setTag(tag);
        _animations[pair.first] = action;
        _tags[pair.first] = tag++;
    }
}

void Animations::runAnimation(cocos2d::Sprite* target, const std::string& name, bool forceRestart) {
    int tag = _tags[name];
    if (!target->getActionByTag(tag) || forceRestart) {
        target->stopAllActions();
        auto animation = _animations.at(name);
        animation->setTag(tag);
        target->runAction(animation);
    }
}