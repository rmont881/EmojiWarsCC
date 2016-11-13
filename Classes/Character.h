//
//  Character.h
//  MyCppGame
//
//  Created by Josh Bodily on 10/18/16.
//
//

#pragma once

#include "2d/CCNode.h"
#include "GamePad.h"
#include "Collision.h"

namespace cocos2d { namespace ui { class Layout; } }
class PickupableInterface;

class Character : public cocos2d::Node, public CollideableInterface {
public:
    CREATE_FUNC(Character);
    virtual ~Character() { }
    virtual bool init() override;
    virtual void update(float dt) override;
    virtual void pause() override { _sprite->pause(); }
    // ICollideable
    virtual const cocos2d::Vec2& getPosition() const override { return Node::getPosition(); }
    virtual const cocos2d::Size& getBounds() const override { return _bounds; }
    virtual void resolveCollision(Collider* collider, const std::vector<cocos2d::Vec2>& collisionVectors) override;
private:
    
    // Display
    cocos2d::Node* _parabolaNode = nullptr;
    cocos2d::Sprite* _sprite = nullptr;
    cocos2d::Size _bounds;
    cocos2d::DrawNode* _debugDraw = nullptr;
    
    // Physics
    cocos2d::Vec2 _velocity;
    cocos2d::Vec2 _oldPosition;
    
    bool _onGround = false;
    float _jumpForces = 0.0f;
    
    ControllerState _previousControllerState;
    PickupableInterface* _heldItem = nullptr;
  int _player_id = 0;

  static int _number_of_players;
};
