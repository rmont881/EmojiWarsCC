//
//  Level.h
//  MyCppGame
//
//  Created by Josh Bodily on 10/23/16.
//
//

#pragma once

#include "2d/CCNode.h"

class Collider;
namespace cocos2d { class TMXTiledMap; }

class Level : public cocos2d::Node {
public:
    CREATE_FUNC(Level);
    virtual bool init() override;
    virtual void load(const std::string& file);
    virtual const std::vector<Collider*>& getColliders() const { return _colliders; }
private:
    bool isCollidableTile(uint32_t gid) const;
    float getFloatPropertyForTile(const uint32_t gid, const std::string& propName) const;
    void createCollidersForTiles();
    void createCollidersFromObjectLayer();
    
    size_t _width, _height;
    cocos2d::TMXTiledMap* _map;
    std::vector<Collider*> _colliders;
    cocos2d::DrawNode* _debugDraw;
};