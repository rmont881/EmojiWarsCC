//
//  IPickupable.h
//  MyCppGame
//
//  Created by Josh Bodily on 10/19/16.
//
//

#pragma once

#include "2d/CCNode.h"
#include "Collision.h"

class PickupableInterface;
namespace Util {
    bool isThrowableItem(PickupableInterface* item);
}

class PickupableInterface : public cocos2d::Node, public CollideableInterface {
public:
    virtual void update(float dt) = 0;
    virtual Collider* getCollideable() = 0;
    virtual void setPosition(const cocos2d::Vec2& pos) override { _oldPosition = _position = pos; cocos2d::Node::setPosition(pos); }
    virtual void setVelocity(const cocos2d::Vec2& vel) { _velocity = vel; }
protected:
    Collider* _collider = nullptr;
    cocos2d::Vec2 _position;
    cocos2d::Vec2 _oldPosition;
    cocos2d::Size _bounds;
    cocos2d::Vec2 _velocity;
    cocos2d::Sprite* _sprite;
};

class Weapon : public PickupableInterface {
public:
    CREATE_FUNC(Weapon);
    bool init() override;
    // pickupable
    virtual void fire() { }
    virtual float canFireIn() { return 0.0f; }
    virtual void update(float dt) override;
    virtual Collider* getCollideable() override { return _collider; }
    // collideable
    virtual const cocos2d::Vec2& getPosition() const override { return _position; }
    virtual const cocos2d::Size& getBounds() const override { return _bounds; }
    virtual void resolveCollision(Collider* collider, const std::vector<cocos2d::Vec2>& collisionVectors) override;
};