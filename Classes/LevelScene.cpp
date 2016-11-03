#include "LevelScene.h"
#include "SimpleAudioEngine.h"
#include "2d/CCTMXTiledMap.h"
#include "ui/UILayout.h"

#include "GamePad.h"
#include "Menu.h"
#include "Level.h"
#include "Character.h"
#include "Collision.h"
#include "Constants.h"
#include "Pickup.h"

USING_NS_CC;

const std::vector<std::string> SPRITE_SHEETS = {"girl", "guy"};

Scene* LevelScene::createScene() {
    auto scene = Scene::create();
    auto layer = LevelScene::create();
    scene->addChild(layer);
    return scene;
}

bool LevelScene::init() {
    if (!Layer::init()) return false;
    
    _pickups.clear();
    
    scheduleUpdate();
    
    initSpriteSheets();
    initAnimations();
    
    // Create the map
    _map = Level::create();
    _map->load("test.tmx");
    addChild(_map);

    // Create and add the character
    _character = Character::create();
    addChild(_character);
    
    Weapon* weapon = Weapon::create();
    _pickups.push_back(weapon);
    addChild(weapon);
    
    _pauseLabel = cocos2d::Label::create("PAUSED", "fonts/ARCADECLASSIC.TTF", 80);
    _pauseLabel->enableOutline(cocos2d::Color4B::WHITE, 8);
    _pauseLabel->setTextColor(cocos2d::Color4B::BLACK);
    _pauseLabel->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _pauseLabel->setPosition(SCREEN_CENTER);
    addChild(_pauseLabel);
    
    return true;
}

void LevelScene::initSpriteSheets() {
    for (const auto& fileName : SPRITE_SHEETS) {
        char name[128];
        sprintf(name, "%s.plist", fileName.c_str());
        cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(name);
        sprintf(name, "%s.png", fileName.c_str());
        auto spriteSheet = cocos2d::SpriteBatchNode::create(name);
        addChild(spriteSheet);
    }
}

void LevelScene::initAnimations() {
    cocos2d::AnimationCache::getInstance()->addAnimationsWithFile("animations.plist");
}

void LevelScene::collisionDetection() {
    // TODO: Broad-phase

    // Collide everything against the level
    for (auto collider : _map->getColliders()) {
        collider->collide(_character);
        for (auto weapon : _pickups) {
            collider->collide(weapon);
        }
    }
    
    // Collide pickups against players
    for (auto pickup : _pickups) {
        pickup->getCollideable()->collide(_character);
    }
}

void LevelScene::update(float dt) {
    if (!_paused) {
        // Update all characters
        _character->update(dt);
        // Update all items
    }
    
    if (!_previousControllerState.start && Util::getControllerState(0).start) {
        _paused = !_paused;
        _pauseLabel->setVisible(_paused);
        _character->pause();
    }
    if (_paused && Util::getControllerState(0).back) {
        cocos2d::Director::getInstance()->replaceScene(MenuScene::createScene());
        return;
    }
    
    collisionDetection();
    _previousControllerState = Util::getControllerState(0);
}
