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
  virtual ~MenuScene();
    virtual void initSpriteSheets();
    virtual void update(float dt);
  cocos2d::Label* labelPOne;
  cocos2d::Label* labelPTwo;
  cocos2d::Label* labelPThree;
  cocos2d::Label* labelPFour;
  bool playerOne = false;
  bool playerTwo = false;
  bool playerThree = false;
  bool playerFour = false;
};

