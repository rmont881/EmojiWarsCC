//
//  Menu.h
//  MyCppGame
//
//  Created by Josh Bodily on 10/19/16.
//
//

#pragma once

#include "cocos2d.h"

class MenuScene : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();
private:
    CREATE_FUNC(MenuScene);
    virtual bool init();
    virtual void update(float dt);
};

