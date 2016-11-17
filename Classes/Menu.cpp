//
//  Menu.cpp
//  MyCppGame
//
//  Created by Josh Bodily on 10/19/16.
//
//

#include "Menu.h"
#include "GamePad.h"
#include "LevelScene.h"
#include "Constants.h"

const std::vector<std::string> SPRITE_SHEETS = {"girl", "guy"};

cocos2d::Scene* MenuScene::createScene() {
    auto scene = cocos2d::Scene::create();
    auto layer = MenuScene::create();
    scene->addChild(layer);
    return scene;
}

bool MenuScene::init() {
    if (!Layer::init()) return false;
    initSpriteSheets();
  Util::setListenForInput(true);
    
    auto label = cocos2d::Label::create("EMOJI\nWARS", "fonts/ARCADECLASSIC.TTF", 90);
    label->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    addChild(label);
    label->setPosition(cocos2d::Vec2(RESOLUTION_WIDTH / 2, RESOLUTION_HEIGHT / 2 + 140));
    
//    label->runAction(cocos2d::FadeOut::create(1.0f));
    label->runAction(cocos2d::RepeatForever::create(cocos2d::Sequence::create(cocos2d::ScaleTo::create(0.5f, 1.1f), cocos2d::ScaleTo::create(0.5f, 1.0f), nullptr)));
    
    for (int i = 1; i < 5; ++i) {
        char buffer[12];
        sprintf(buffer, "Player %d", i);
        if (i == 1) buffer[8] = '\0';
        auto label = cocos2d::Label::create(buffer, "fonts/ARCADECLASSIC.TTF", 48);
        addChild(label);
        label->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        label->setPosition(cocos2d::Vec2(RESOLUTION_WIDTH / 3, 350 - i * 50));
      
        char join_buffer[12];
        sprintf(join_buffer, "Press ");
        auto join_label = cocos2d::Label::create(join_buffer, "fonts/ARCADECLASSIC.TTF", 48);
        join_label->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        join_label->setPosition(cocos2d::Vec2((RESOLUTION_WIDTH / 3) * 2, 350 - i * 50));
        join_label->runAction(cocos2d::RepeatForever::create(cocos2d::Sequence::create(cocos2d::ScaleTo::create(0.5f, 1.1f), cocos2d::ScaleTo::create(0.5f, 1.0f), nullptr)));
        auto sprite = cocos2d::Sprite::createWithSpriteFrameName("a_button.png");
      sprite->getTexture()->setAliasTexParameters();
        sprite->setPosition(join_label->getPositionX() + 110.0f, 350 - i * 50);
        sprite->runAction(cocos2d::RepeatForever::create(cocos2d::Sequence::create(cocos2d::ScaleTo::create(0.5f, 1.1f), cocos2d::ScaleTo::create(0.5f, 1.0f), nullptr)));
        addChild(join_label);
        addChild(sprite);
      //Total hack, clean this up later
      if(i==1) {
        labelPOne = join_label;
      } else if(i==2) {
        labelPTwo = join_label;
      } else if(i==3) {
        labelPThree = join_label;
      } else if(i==4) {
        labelPFour = join_label;
      }
    }
    
    scheduleUpdate();
    
    return true;
}

MenuScene::~MenuScene() {
  Util::setListenForInput(false);
}

void MenuScene::initSpriteSheets() {
  for (const auto& fileName : SPRITE_SHEETS) {
    char name[128];
    sprintf(name, "%s.plist", fileName.c_str());
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(name);
    sprintf(name, "%s.png", fileName.c_str());
    auto spriteSheet = cocos2d::SpriteBatchNode::create(name);
    addChild(spriteSheet);
  }
}

void MenuScene::update(float dt) {
    if (Util::getControllerState(0).start) {
        cocos2d::Director::getInstance()->replaceScene(LevelScene::createScene());
    }
  if(!playerOne) {
    if(Util::getControllerState(0).controller != nullptr) {
      playerOne = true;
      labelPOne->setString("Joined");
    }
  }
  if(!playerTwo) {
    if(Util::getControllerState(1).controller != nullptr) {
      playerTwo = true;
      labelPTwo->setString("Joined");
    }
  }
  if(!playerThree) {
    if(Util::getControllerState(2).controller != nullptr) {
      playerThree = true;
      labelPThree->setString("Joined");
    }
  }
  if(!playerFour) {
    if(Util::getControllerState(3).controller != nullptr) {
      playerFour = true;
      labelPFour->setString("Joined");
    }
  }
  
}
