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
#include "audio/include/SimpleAudioEngine.h"

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

  _drawBackground();

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

  _drawStatusBar();

  return true;
}

void LevelScene::_drawBackground() {
  auto layout = cocos2d::ui::Layout::create();
  layout->setContentSize(this->getContentSize());
  layout->setBackGroundColor(cocos2d::Color3B(165, 229, 255), cocos2d::Color3B(227, 247, 255));
  layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::GRADIENT);
  addChild(layout);
  
  auto bgLabel = cocos2d::Label::create();
  bgLabel->setSystemFontSize(48);
  bgLabel->setString("🏢🏬 🏣🏤🏨🏦🏢");
  bgLabel->setColor(cocos2d::Color3B::WHITE);
  bgLabel->setScale(2.0f);
  bgLabel->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
  bgLabel->setPosition(cocos2d::Point(RESOLUTION_WIDTH * 0.5, -20.0f));
  bgLabel->setOpacity(100);
  addChild(bgLabel);
  
  auto label = cocos2d::Label::create();
  label->setSystemFontSize(48);
  label->setString("");
  label->setScale(2.0f);
  label->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
  label->setPosition(SCREEN_CENTER);
  label->setOpacity(255);
  addChild(label);
}

void LevelScene::_drawStatusBar() {
  for (int i = 0; i < 4; ++i) {
    auto _label1 = cocos2d::Label::create("Player " + std::to_string(i + 1), "fonts/ARCADECLASSIC.TTF", 36);
    _label1->enableOutline(cocos2d::Color4B::WHITE, 2);
    _label1->setTextColor(cocos2d::Color4B::BLACK);
    _label1->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _label1->setPosition(SCREEN_TOP_LEFT + cocos2d::Vec2(i * RESOLUTION_WIDTH / 4.0, 0.0));
    _label1->setAnchorPoint(cocos2d::Vec2::ANCHOR_TOP_LEFT);
    
    auto label2 = cocos2d::Label::create();
    label2->setSystemFontSize(24);
    label2->setString("❤️❤️❤️");
    label2->setPosition(_label1->getPosition() + cocos2d::Vec2(150, 0));
    label2->setAnchorPoint(cocos2d::Vec2::ANCHOR_TOP_LEFT);
    addChild(label2);
    addChild(_label1);
  }
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
