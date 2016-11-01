//
//  IPickupable.cpp
//  MyCppGame
//
//  Created by Josh Bodily on 10/19/16.
//
//

#include "Pickup.h"
#include "Constants.h"

const cocos2d::Vec2 GRAVITY(0.0f, GRAVITY_Y);

namespace Util {
    bool isThrowableItem(PickupableInterface* item) { return true; }
}

bool Weapon::init() {
    if (!cocos2d::Node::init()) return false;
    
    _sprite = cocos2d::Sprite::createWithSpriteFrameName("Untitled.png");
    _sprite->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
    _sprite->getTexture()->setAliasTexParameters();
    addChild(_sprite);
    
    _oldPosition = cocos2d::Vec2(250.0f, 250.0f);
    setPosition(_oldPosition);
    
    _collider = new BoxCollider(_position, cocos2d::Size(20.0f, 20.0f), this);
    _collider->setFlags(COLLIDE_ALL_SIDES);
    _collider->setTag(WEAPON_TAG);
    
    _bounds = cocos2d::Size(10.0f, 10.0f);
    
    return true;
}

void Weapon::resolveCollision(Collider* collider, const std::vector<cocos2d::Vec2>& collisionVectors) {
    if (_collider->getFlags() == 0) return;
    
    auto position = getPosition();
    auto closest = Util::findClosestResolution(collisionVectors, position, _oldPosition);
    
    position += closest;
    setPosition(position);
    
    if (closest.getNormalized().dot(cocos2d::Vec2::UNIT_Y) > 0.5f) {
        _velocity.y = 0.0f;
    }
}

void Weapon::update(float dt) {
    if (_collider->getFlags() == 0) return;
    
    dt = FIXED_TIME_STEP;
    _oldPosition = _position;
    
    cocos2d::Vec2 forces = GRAVITY;
    _position += _velocity * dt;
    _velocity += ( forces / 1.0f /* mass */ ) * dt;
    
    setPosition(_position);
    _collider->setPosition(_position);
}