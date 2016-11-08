//
//  Collision.cpp
//  MyCppGame
//
//  Created by Josh Bodily on 10/19/16.
//
//

#include "Collision.h"

namespace Util {
    cocos2d::Vec2 findClosestResolution(const std::vector<cocos2d::Vec2>& collisionVectors, const cocos2d::Vec2& position, const cocos2d::Vec2& lastPosition) {
        cocos2d::Vec2 closest = cocos2d::Vec2::ZERO;
        float min = 99999999.9f;
        for (const auto& vec : collisionVectors) {
            cocos2d::Vec2 diff = (position + vec) - lastPosition;
            if (diff.lengthSquared() < min) {
                min = diff.lengthSquared();
                closest = vec;
            }
        }
        return closest;
    }
}

namespace {
    // see http://www.metanetsoftware.com/technique/tutorialA.html#section2
    std::vector<cocos2d::Vec2> SATAABB(float x1, float y1, float hw1, float hh1, float x2, float y2, float hw2, float hh2, std::bitset<32> flags) {
        std::vector<cocos2d::Vec2> results;
        
        // Check for early out, i.e. no collision if there is at least one separating axis
        float dx = x1 < x2 ? (x1 + hw1) - (x2 - hw2) : (x2 + hw2) - (x1 - hw1);
        if (dx <= 0.0)
            return results;
        float dy = y2 > y1 ? (y1 + hh1) - (y2 - hh2) : (y2 + hh2) - (y1 - hh1);
        if (dy <= 0.0)
            return results;
        
        if (flags.test(1) && x1 < x2)
            results.push_back(cocos2d::Vec2(dx, 0));
        if (flags.test(3) && x2 <= x1)
            results.push_back(cocos2d::Vec2(-dx, 0));
        if (flags.test(0) && y1 < y2)
            results.push_back(cocos2d::Vec2(0, dy));
        if (flags.test(2) && y2 <= y1)
            results.push_back(cocos2d::Vec2(0, -dy));
        
        return results;
    }
}

void BoxCollider::collide(CollideableInterface* collideable) {
    auto bounds = collideable->getBounds();
    auto rect = this->getBounds();
    float x1 = rect.getMidX();
    float y1 = rect.getMidY();
    float hw1 = rect.size.width * 0.5f;
    float hh1 = rect.size.height * 0.5f;
    float x2 = collideable->getPosition().x;
    float y2 = collideable->getPosition().y + bounds.height * 0.5f;
    collideable->resolveCollision(this, SATAABB(x1, y1, hw1, hh1, x2, y2, bounds.width * 0.5f, bounds.height * 0.5f, this->getFlags()));
}

void TriangleCollider::collide(CollideableInterface* collideable) {
    auto bounds = collideable->getBounds();
    auto rect = this->getBounds();
    float x1 = rect.getMidX();
    float y1 = rect.getMidY();
    float hw1 = rect.size.width * 0.5f;
    float hh1 = rect.size.height * 0.5f;
    float x2 = collideable->getPosition().x;
    float y2 = collideable->getPosition().y + bounds.height * 0.5f;
    auto proj = SATAABB(x1, y1, hw1, hh1, x2, y2, bounds.width * 0.5f, bounds.height * 0.5f, this->getFlags());
    if (proj.empty()) return;
    
    // TODO: This stops hickups on contiguous ramps
//    proj.clear();
    auto it = proj.begin();
    for (; it != proj.end(); ++it) {
        if (it->x < 0) {
            proj.erase(it);
            break;
        }
    }
    
    
    auto w = this->_bounds.size.width;
    auto h = this->_bounds.size.height;
    
    if (_orientation == TriangleOrientation::FIRST_QUADRANT) {
        cocos2d::Vec2 perp(h, w);
        perp.normalize();
        float x = (collideable->getPosition().x - collideable->getBounds().width * 0.5f) - (this->_bounds.origin.x + this->_bounds.size.width);
        float y = (collideable->getPosition().y) - (this->_bounds.origin.y);
        auto projection = cocos2d::Vec2(x, y).project(perp);
        if (projection.x < 0.0f) {
            proj.push_back(-projection);
            collideable->resolveCollision(this, proj);
        }
    }
    if (_orientation == TriangleOrientation::SECOND_QUADRANT) {
        cocos2d::Vec2 perp(-h, w);
        perp.normalize();
        float x = (collideable->getPosition().x + collideable->getBounds().width * 0.5f) - (this->_bounds.origin.x);
        float y = (collideable->getPosition().y) - (this->_bounds.origin.y);
        auto projection = cocos2d::Vec2(x, y).project(perp);
        if (projection.x > 0.0f) {
            proj.push_back(-projection);
            collideable->resolveCollision(this, proj);
        }
    }
    if (_orientation == TriangleOrientation::THIRD_QUADRANT) {
        cocos2d::Vec2 perp(-h, -w);
        perp.normalize();
        float x = (collideable->getPosition().x + collideable->getBounds().width * 0.5f) - (this->_bounds.origin.x + this->_bounds.size.width);
        float y = (collideable->getPosition().y + collideable->getBounds().height) - (this->_bounds.origin.y);
        auto projection = cocos2d::Vec2(x, y).project(perp);
        if (projection.x > 0.0f) {
            proj.push_back(-projection);
            collideable->resolveCollision(this, proj);
        }
    }
    
}

void TriangleCollider::setFlags(std::bitset<32> flags) {
    if (_orientation == TriangleOrientation::SECOND_QUADRANT) {
//        flags.reset(0); // TOP
//        flags.reset(2); // LEFT
    }
    _flags = flags;
}