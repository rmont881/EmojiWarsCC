//
//  Collision.h
//  MyCppGame
//
//  Created by Josh Bodily on 10/19/16.
//
//

#pragma once

class Collider;

enum ColliderFlags {
    TOP = 0x1,
    RIGHT = 0x2,
    BOTTOM = 0x4,
    LEFT = 0x8
};

enum class TriangleOrientation {
    FIRST_QUADRANT,
    SECOND_QUADRANT,
    THIRD_QUADRANT,
    FOURTH_QUADRANT
};

namespace Util {
    cocos2d::Vec2 findClosestResolution(const std::vector<cocos2d::Vec2>& collisionVectors, const cocos2d::Vec2& position, const cocos2d::Vec2& lastPosition);
}

class CollideableInterface {
public:
    virtual const cocos2d::Vec2& getPosition() const = 0;
    virtual const cocos2d::Size& getBounds() const = 0;
    virtual void resolveCollision(Collider* collider, const std::vector<cocos2d::Vec2>& collisionVectors) { }
};

class Collider {
public:
    Collider(const cocos2d::Rect& bounds, void* parent) : _bounds(bounds), _parent(parent) { }
    virtual const cocos2d::Rect& getBounds() = 0;
    virtual void collide(CollideableInterface*) = 0;
    virtual void setPosition(const cocos2d::Vec2& pos) { _bounds.origin = pos; }
    virtual void setFlags(int flags) { _flags = flags; }
    virtual int getFlags() const { return _flags; }
    virtual int getTag() const { return _tag; }
    virtual void setTag(int tag) { _tag = tag; }
    virtual void* getParent() const { return _parent; }
protected:
    void* _parent;
    cocos2d::Rect _bounds;
    int _tag = 0;
    int _flags = 0;
};

class BoxCollider : public Collider {
public:
    BoxCollider(const cocos2d::Vec2& pos, const cocos2d::Size& size, void* parent = nullptr) : Collider(cocos2d::Rect(pos, size), parent) { }
    BoxCollider(const cocos2d::Rect& bounds, void* parent = nullptr) : Collider(bounds, parent) { }
    const cocos2d::Rect& getBounds() override { return _bounds; }
    virtual void collide(CollideableInterface* character) override;
};

class TriangleCollider : public Collider {
public:
    TriangleCollider(const cocos2d::Vec2& pos, const cocos2d::Size& size, TriangleOrientation orientation) : Collider(cocos2d::Rect(pos, size), nullptr), _orientation(orientation) { }
    TriangleCollider(const cocos2d::Rect& bounds, TriangleOrientation orientation) : Collider(bounds, nullptr), _orientation(orientation) { }
    const cocos2d::Rect& getBounds() override { return _bounds; }
    virtual void collide(CollideableInterface* character) override;
private:
    TriangleOrientation _orientation;
};
