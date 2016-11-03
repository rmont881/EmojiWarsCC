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
    std::vector<cocos2d::Vec2> SATAABB(float x1, float y1, float hw1, float hh1, float x2, float y2, float hw2, float hh2, int flags = 0) {
        std::vector<cocos2d::Vec2> results;
        
        // Check for early out, i.e. no collision if there is at least one separating axis
        float dx = x1 < x2 ? (x1 + hw1) - (x2 - hw2) : (x2 + hw2) - (x1 - hw1);
        if (dx <= 0.0)
            return results;
        float dy = y2 > y1 ? (y1 + hh1) - (y2 - hh2) : (y2 + hh2) - (y1 - hh1);
        if (dy <= 0.0)
            return results;
        
        if (flags & 0x2 && x1 < x2)
            results.push_back(cocos2d::Vec2(dx, 0));
        if (flags & 0x8 && x2 <= x2)
            results.push_back(cocos2d::Vec2(-dx, 0));
        if (flags & 0x1 && y1 < y2)
            results.push_back(cocos2d::Vec2(0, dy));
        if (flags & 0x4 && y2 <= y1)
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
    
    auto w = this->_bounds.size.width;
    auto h = this->_bounds.size.height;
    cocos2d::Vec2 hypotenuse(w, -h);
    hypotenuse.normalize();
    cocos2d::Vec2 perp = hypotenuse.getPerp();
    
    cocos2d::Vec2 characterBounds(collideable->getPosition().x - collideable->getBounds().width * 0.5f - this->_bounds.origin.x, collideable->getPosition().y - this->_bounds.origin.y - h);
    cocos2d::Vec2 characterProjection = characterBounds.project(perp);
    cocos2d::Vec2 triangleBounds(0, 0);
    cocos2d::Vec2 triangleProjection = triangleBounds.project(perp);
    
    if (characterProjection.x < triangleProjection.x) {
        cocos2d::Vec2 hypotenuse_projection(triangleProjection.x - characterProjection.x, triangleProjection.y - characterProjection.y);
//        std::vector<cocos2d::Vec2> projs;
        proj.push_back(hypotenuse_projection);
        collideable->resolveCollision(this, proj);
        return;
    } else {
        return;
    }
}