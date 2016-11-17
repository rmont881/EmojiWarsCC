//
//  Character.cpp
//  MyCppGame
//
//  Created by Josh Bodily on 10/18/16.
//
//

#include "Animations.h"
#include "Character.h"
#include "Constants.h"
#include "GamePad.h"
#include "Level.h"
#include "Pickup.h"
#include "ui/UILayout.h"

const cocos2d::Vec2 GRAVITY(0.0f, GRAVITY_Y);

int Character::_number_of_players = 0;
bool Character::init() {
  if (!Node::init())
    return false;

  _player_id = _number_of_players++;
#if KEYBOARD_ONLY
  extern std::map<void *, ControllerState> controllerStates;
  controllerStates.insert({0, ControllerState()});
  auto listener = cocos2d::EventListenerKeyboard::create();
  listener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode,
                                  cocos2d::Event *event) {
    ControllerState &keyboard_state = Util::getControllerState(_player_id);
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_A ||
        keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
      _velocity.x = 0.0f;
      keyboard_state.left = true;
      keyboard_state.right = false;
    }
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_D ||
        keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
      _velocity.x = 0.0f;
      keyboard_state.right = true;
      keyboard_state.left = false;
    }
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_S ||
        keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
      keyboard_state.down = true;
    }
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE) {
      keyboard_state.a = true;
    }
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ENTER) {
      keyboard_state.start = true;
    }
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_SHIFT) {
      keyboard_state.x = true;
    }
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
      keyboard_state.right_stick.x = 6000.0f;
    }
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
      keyboard_state.right_stick.x = -6000.0f;
    }
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW) {
      keyboard_state.right_stick.y = 6000.0f;
    }
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
      keyboard_state.right_stick.y = -6000.0f;
    }
  };
  listener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode,
                                   cocos2d::Event *event) {

    ControllerState &keyboard_state = Util::getControllerState(_player_id);
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_A ||
        keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
      keyboard_state.left = false;
    }
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_D ||
        keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
      keyboard_state.right = false;
    }
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_S ||
        keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
      keyboard_state.down = false;
    }
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE) {
      keyboard_state.a = false;
    }
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ENTER) {
      keyboard_state.start = false;
    }
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_SHIFT) {
      keyboard_state.x = false;
    }
  };

  cocos2d::Director::getInstance()
      ->getEventDispatcher()
      ->addEventListenerWithFixedPriority(listener, -1);
#endif

  auto label = cocos2d::Label::create("P1", "fonts/ARCADECLASSIC.TTF", 48);
  addChild(label);
  label->enableOutline(cocos2d::Color4B::WHITE, 2);
  label->setTextColor(cocos2d::Color4B::BLACK);
  label->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
  label->setPositionY(100);

  _sprite =
      cocos2d::Sprite::createWithSpriteFrameName("guy_walking_20_001.png");
  _sprite->setScale(0.75f);
  _sprite->getTexture()->setAliasTexParameters();
  _sprite->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
  addChild(_sprite);
  //  _sprite->setColor(cocos2d::Color3B::RED);

  _bounds = cocos2d::Size(20.0f, 50.0f);
#ifdef DEBUG_DRAW
  _debugDraw = cocos2d::DrawNode::create();
  _debugDraw->drawRect(cocos2d::Vec2(-_bounds.width * 0.5f, 0),
                       cocos2d::Vec2(_bounds.width * 0.5f, _bounds.height),
                       cocos2d::Color4F::BLUE);
  addChild(_debugDraw);
#endif

  // plot a parabola for throw path
  _parabolaNode = Node::create();
  addChild(_parabolaNode);
  for (int i = 0; i < 20; ++i) {
    auto sprite = cocos2d::Sprite::createWithSpriteFrameName("dot.png");
    sprite->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprite->getTexture()->setAliasTexParameters();
    _parabolaNode->addChild(sprite);
  }

  cocos2d::Vec2 position(272, 400);
  setPosition(position);
  _oldPosition = position;
  _velocity = cocos2d::Vec2::ZERO;

  return true;
}

void Character::resolveCollision(
    Collider *collider, const std::vector<cocos2d::Vec2> &collisionVectors) {

  auto position = getPosition();
  auto closest =
      Util::findClosestResolution(collisionVectors, position, _oldPosition);

  // Collision with level
  if (collider->getTag() == 0) {
    if (closest.getNormalized().dot(cocos2d::Vec2::UNIT_Y) > 0.5f &&
        _velocity.y < 0.0f) {
      _velocity.y = 0.0f;
      _surfaceNormal = closest.getNormalized();
    }
    if (closest.getNormalized().equals(-cocos2d::Vec2::UNIT_Y)) {
      _velocity.y = 0.0f;
    }
    if (fabs(closest.getNormalized().dot(cocos2d::Vec2::UNIT_X)) > 0.9f) {
      _velocity.x = 0.0f;
      _speed = 0.0f;
      //      CCLOG("%f", _velocity.x);
    }
    position += closest;
    setPosition(position);
  }

  // Pickups
  else if (collider->getTag() == WEAPON_TAG &&
           !closest.equals(cocos2d::Vec2::ZERO)) {
    if (Util::getControllerState(_player_id).down &&
        Util::getControllerState(_player_id).x) {
      collider->setFlags(0);
      _heldItem = static_cast<PickupableInterface *>(collider->getParent());
    }
  }
}

void Character::update(float dt) {
  dt = FIXED_TIME_STEP;

  cocos2d::Vec2 forces = GRAVITY;

  if (std::fabs(_speed) > 20.0 && _onGround) {
    _sprite->setFlippedX(_velocity.x < 0.0);
    Animations::runAnimation(_sprite, "guy_walk");
    CCLOG("%f", _speed);
  } else if (_onGround) {
    Animations::runAnimation(_sprite, "guy_idle");
  }

  if (_velocity.y < -700) {
    Animations::runAnimation(_sprite, "guy_falling");
  } else {
    _sprite->stopActionByTag(3);
  }

  // Jumping
  bool jumping = false;
  if (!_previousControllerState.a && Util::getControllerState(_player_id).a &&
      _onGround) {
    CCLOG("Jumping");
    _velocity.y = 0.0f;
    _jumpForces = 7000.0f;
    Animations::runAnimation(_sprite, "guy_jumping");
    jumping = true;
  }
  if (Util::getControllerState(_player_id).a)
    _jumpForces *= 0.9f;
  else
    _jumpForces *= 0.5f;

  // ray cast down
  float rayCast = Level::_currentLevel->rayCast(
      cocos2d::Vec2(getPositionX() - _bounds.width * 0.45f, getPositionY()));
  float rayCast2 = Level::_currentLevel->rayCast(
      cocos2d::Vec2(getPositionX() + _bounds.width * 0.45f, getPositionY()));
  if (_velocity.y <= 0 && (rayCast < 12.0 || rayCast2 < 12.0f) && !jumping) {
    setPositionY(getPositionY() - std::min(rayCast, rayCast2));
    _onGround = true;
  } else {
    _onGround = false;
  }

  // Accumulate forces and update
  forces += cocos2d::Vec2(0.0f, _jumpForces);
  _oldPosition = getPosition();
  setPosition(getPosition() + _velocity * dt);
  _velocity += (forces / 1.0f) * dt;

  // Left / Right Movement
  if (Util::getControllerState(_player_id).left) {
    if (!_previousControllerState.left)
      _speed = 0.0f;
    _speed += 15.0f;
    _velocity.x = -_speed;
  }
  if (Util::getControllerState(_player_id).right) {
    if (!_previousControllerState.right)
      _speed = 0.0f;
    _speed += 15.0f;
    _velocity.x = _speed;
  }
  _speed = cocos2d::clampf(_speed, 0.0f, 250.0f);

  // Ground Friction
  if (!Util::getControllerState(_player_id).left &&
      !Util::getControllerState(_player_id).right && _onGround) {
    _speed *= 0.5f;
    _velocity.x *= 0.5f;
  }

  // Draw parabolic arc for aiming thrown items
  if (Util::getControllerState(_player_id).right_stick.length() > 8000 &&
      _heldItem != nullptr && Util::isThrowableItem(_heldItem)) {

    cocos2d::Vec2 points[20];
    float t = 0.1f;
    for (int i = 0; i < 20; ++i) {
      points[i].x =
          0.0f + Util::getControllerState(_player_id).right_stick.x * t * 0.02;
      points[i].y =
          32.0f +
          Util::getControllerState(_player_id).right_stick.y * t * -0.03 +
          -1000 * t * t;
      t += 0.1f;
    }
    int i = 0;
    for (auto &child : _parabolaNode->getChildren()) {
      if (points[i].x > 800 || points[i].y > 800)
        child->setVisible(false);
      else
        child->setVisible(true);

      child->setPosition(points[i++]);
    }
    _parabolaNode->setVisible(true);

  } else {
    _parabolaNode->setVisible(false);
  }

  // Pick-ups, display as carried
  if (_heldItem != nullptr) {
    _heldItem->setPosition(getPosition());
  }

  // Throw item
  if (Util::getControllerState(_player_id).right_stick.length() > 8000 &&
      Util::getControllerState(_player_id).x && !_previousControllerState.x &&
      _heldItem != nullptr) {
    float x = Util::getControllerState(_player_id).right_stick.x * 0.02;
    float y = Util::getControllerState(_player_id).right_stick.y * 0.02;
    _heldItem->setVelocity(cocos2d::Vec2(x, y));
    _heldItem->getCollideable()->setFlags(COLLIDE_ALL_SIDES);
    _heldItem = nullptr;
  }

  // Finalize update
  _previousControllerState = Util::getControllerState(_player_id);
}
