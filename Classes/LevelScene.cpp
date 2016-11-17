#include "2d/CCTMXTiledMap.h"
#include "LevelScene.h"
#include "SimpleAudioEngine.h"
#include "ui/UILayout.h"

#include "Animations.h"
#include "Character.h"
#include "Collision.h"
#include "Constants.h"
#include "GamePad.h"
#include "Level.h"
#include "Menu.h"
#include "Pickup.h"

USING_NS_CC;

const std::vector<std::string> SPRITE_SHEETS = {"girl", "guy"};

Scene *LevelScene::createScene() {
  auto scene = Scene::create();
  auto layer = LevelScene::create();
  scene->addChild(layer);
  return scene;
}

bool LevelScene::init() {
  if (!Layer::init())
    return false;

  _pickups.clear();

  scheduleUpdate();

  initSpriteSheets();
  initAnimations();

  // Create the map
  _map = Level::create();
  _map->load("test.tmx");
  addChild(_map);

  // Create and add the characters
  for (auto &character : _characters) {
    character = Character::create();
    addChild(character);
  }

  Weapon *weapon = Weapon::create();
  _pickups.push_back(weapon);
  addChild(weapon);

  _pauseLabel = cocos2d::Label::create("PAUSED", "fonts/ARCADECLASSIC.TTF", 80);
  _pauseLabel->enableOutline(cocos2d::Color4B::WHITE, 8);
  _pauseLabel->setTextColor(cocos2d::Color4B::BLACK);
  _pauseLabel->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
  _pauseLabel->setPosition(SCREEN_CENTER);
  _pauseLabel->setVisible(false);
  _pauseLabel->setOpacity(200);
  addChild(_pauseLabel);

  return true;
}

void LevelScene::initSpriteSheets() {
  for (const auto &fileName : SPRITE_SHEETS) {
    char name[128];
    sprintf(name, "%s.plist", fileName.c_str());
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(name);
    sprintf(name, "%s.png", fileName.c_str());
    auto spriteSheet = cocos2d::SpriteBatchNode::create(name);
    addChild(spriteSheet);
  }
}

void LevelScene::initAnimations() {
  Animations::loadAnimations("animations.plist");
}

void LevelScene::collisionDetection() {
  // TODO: Broad-phase

  // Collide everything against the level
  for (auto collider : _map->getColliders()) {
    for (auto character : _characters) {
      collider->collide(character);
    }
    for (auto weapon : _pickups) {
      collider->collide(weapon);
    }
  }

  // Collide pickups against players
  for (auto pickup : _pickups) {
    for (auto character : _characters) {
      pickup->getCollideable()->collide(character);
    }
  }
}

void LevelScene::update(float dt) {
  if (!_paused) {
    // Update all characters
    for (auto character : _characters) {
      character->update(dt);
    }
    // Update all pickups
    for (auto pickup : _pickups) {
      pickup->update(dt);
    }
  }

  for (int character_num = 0; character_num < NUM_PLAYERS; ++character_num) {
    if (!_previousControllerState[character_num].start &&
        Util::getControllerState(character_num).start) {
      _paused = !_paused;
      _pauseLabel->setVisible(_paused);
      break;
    }
  }

  if (_paused) {
    for (auto character : _characters) {
      character->pause();
    }
  }

  if (_paused && Util::getControllerState(0).back) {
    cocos2d::Director::getInstance()->replaceScene(MenuScene::createScene());
    return;
  }

  collisionDetection();
  for (int character_num = 0; character_num < NUM_PLAYERS; ++character_num) {
    _previousControllerState[character_num] =
        Util::getControllerState(character_num);
  }
}
