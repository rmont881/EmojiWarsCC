//
//  Level.cpp
//  MyCppGame
//
//  Created by Josh Bodily on 10/23/16.
//
//

#include "Level.h"
#include "Constants.h"
#include "Collision.h"

bool Level::init() {
    if (!cocos2d::Node::init()) return false;
#ifdef DEBUG_DRAW
    _debugDraw = cocos2d::DrawNode::create();
    addChild(_debugDraw);
#endif
    return true;
}

namespace {
    Collider* getColliderForTile(uint32_t gid, int row, int column) {
        return new BoxCollider(cocos2d::Rect(column * 32, (19 - row) * 32, 32, 32));
    }
    bool isCollidableTile(uint32_t gid);
}

void Level::load(const std::string &file) {
    _colliders.clear();
    _map = cocos2d::TMXTiledMap::create(file);
    addChild(_map);
    
    _width = _map->getMapSize().width;
    _height = _map->getMapSize().height;
    
    createCollidersForTiles();
    createCollidersFromObjectLayer();
}

void Level::createCollidersForTiles() {
    auto layer = _map->getLayer("Tiles");
    for (int row = 0; row < _height; ++row) {
        for (int column = 0; column < _width; ++column) {
            uint32_t gid = layer->getTileGIDAt(cocos2d::Vec2(column, row));
            if (!isCollidableTile(gid))
                continue;
            
            Collider* collider = getColliderForTile(gid, row, column);
            
            int flags = 0x0;
            if (row - 1 > 1 && layer->getTileGIDAt(cocos2d::Vec2(column, row - 1)) != 35) { flags |= TOP; }
            if (column + 1 < _width - 1 && layer->getTileGIDAt(cocos2d::Vec2(column + 1, row)) != 35) { flags |= RIGHT; }
            if (row + 1 < _height - 1 && layer->getTileGIDAt(cocos2d::Vec2(column, row + 1)) != 35) { flags |= BOTTOM; }
            if (column - 1 > 1 && layer->getTileGIDAt(cocos2d::Vec2(column - 1, row)) != 35) { flags |= LEFT; }
            collider->setFlags(flags);
            
            _colliders.push_back(collider);
        }
    }
}

bool Level::isCollidableTile(uint32_t gid) const {
    return gid == 35;
}

float Level::getFloatPropertyForTile(const uint32_t gid, const std::string& propName) const {
    auto props = _map->getPropertiesForGID(gid);
    return props.asValueMap().at(propName).asFloat();
}

void Level::createCollidersFromObjectLayer() {
    auto objectMap = _map->getObjectGroup("Objects");
    auto objects = objectMap->getObjects();
    for (auto object : objects) {
        auto valueMap = object.asValueMap();
        auto width = valueMap.at("width").asFloat();
        auto height = valueMap.at("height").asFloat();
        auto x = valueMap.at("x").asFloat();
        auto y = valueMap.at("y").asFloat();
#ifdef DEBUG_DRAW
        _debugDraw->setLocalZOrder(1);
        _debugDraw->drawRect(cocos2d::Vec2(x, y), cocos2d::Vec2(x + width, y + height), cocos2d::Color4F::GREEN);
#endif
        Collider* collider = new BoxCollider(cocos2d::Rect(x, y, width, height));
        collider->setFlags(COLLIDE_ALL_SIDES);
        _colliders.push_back(collider);
    }
}

