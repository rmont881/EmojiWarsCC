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

cocos2d::Scene* MenuScene::createScene() {
    auto scene = cocos2d::Scene::create();
    auto layer = MenuScene::create();
    scene->addChild(layer);
    return scene;
}

bool MenuScene::init() {
    if (!Layer::init()) return false;
    
    auto label = cocos2d::Label::create("EMOJI\nWARS", "fonts/ARCADECLASSIC.TTF", 100);
    label->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    addChild(label);
    auto count = label->getChildrenCount();
    label->setPosition(cocos2d::Vec2(1024 / 2, 768 / 2 + 200));
    
//    label->runAction(cocos2d::FadeOut::create(1.0f));
    label->runAction(cocos2d::RepeatForever::create(cocos2d::Sequence::create(cocos2d::ScaleTo::create(0.5f, 1.1f), cocos2d::ScaleTo::create(0.5f, 1.0f), nullptr)));
    
    for (int i = 1; i < 5; ++i) {
        char buffer[10];
        sprintf(buffer, "%d Players", i);
        if (i == 1) buffer[8] = '\0';
        auto label = cocos2d::Label::create(buffer, "fonts/ARCADECLASSIC.TTF", 48);
        addChild(label);
        auto count = label->getChildrenCount();
        label->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        label->setPosition(cocos2d::Vec2(1024 / 2, 450 - i * 50));
    }
    
    scheduleUpdate();
    
    return true;
}

void MenuScene::update(float dt) {
    if (Util::getControllerState(0).start) {
        cocos2d::Director::getInstance()->replaceScene(LevelScene::createScene());
    }
}
