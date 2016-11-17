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
    _currentLevel = this;
    return true;
}

Level* Level::_currentLevel = nullptr;

namespace {
    Collider* getColliderForTile(uint32_t gid, int row, int column) {
        if (gid == 35)
            return new BoxCollider(cocos2d::Rect(column * 32, (19 - row) * 32, 32, 32));
        else if (gid == 36)
            return new TriangleCollider(cocos2d::Rect(column * 32, (19 - row) * 32, 32, 32), TriangleOrientation::FIRST_QUADRANT);
        else if (gid == 37)
            return new TriangleCollider(cocos2d::Rect(column * 32, (19 - row) * 32, 32, 32), TriangleOrientation::THIRD_QUADRANT);
        else if (gid == 38)
            return new TriangleCollider(cocos2d::Rect(column * 32, (19 - row) * 32, 32, 32), TriangleOrientation::SECOND_QUADRANT);
        else if (gid == 39)
            return new TriangleCollider(cocos2d::Rect(column * 32, (19 - row) * 32, 32, 32), TriangleOrientation::FOURTH_QUADRANT);
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

float Level::rayCast(const cocos2d::Point& start) const {
//  auto layer = _map->getLayer("Tiles");
//  for (int row = 0; row < _height; ++row) {
//    for (int column = 0; column < _width; ++column) {
  auto layer = _map->getLayer("Tiles");
  int column = start.x / 32;
  int row = 19 - start.y / 32;
  while (row < 20) {
    uint32_t gid = layer->getTileGIDAt(cocos2d::Vec2(column, row));
    if (gid == 35 || gid == 37 || gid == 39) {
      return start.y - (20 - row) * 32.0f;
    }
    // down-slope left
    if (gid == 36) {
      return start.y - (20 - row) * 32.0f + fmod(start.x, 32.0f);
    }
    // down-slope right
    if (gid == 38) {
      return start.y - (20 - row) * 32.0f + (32.0 - fmod(start.x, 32.0f));
    }
    row++;
  }
//    }
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
            // top
            if (row - 1 > 1 && !isCollidableTile(layer->getTileGIDAt(cocos2d::Vec2(column, row - 1)))) { flags |= TOP; }
            // bottom
            if (row + 1 < _height - 1 && !isCollidableTile(layer->getTileGIDAt(cocos2d::Vec2(column, row + 1)))) { flags |= BOTTOM; }
            // left
            if (gid == 35) {
                if (column - 1 > 1 && !isCollidableTile(layer->getTileGIDAt(cocos2d::Vec2(column - 1, row)))) { flags |= LEFT; }
            } else if (gid == 38) {
                if (column - 1 > 1 && row + 1 < _height - 1 && !isCollidableTile(layer->getTileGIDAt(cocos2d::Vec2(column - 1, row + 1)))) { flags |= LEFT; }
            }
            // right
            if (gid == 35) {
                if (column + 1 < _width - 1 && !isCollidableTile(layer->getTileGIDAt(cocos2d::Vec2(column + 1, row)))) { flags |= RIGHT; }
            } else if (gid == 36) {
                if (column + 1 < _width && row + 1 < _height - 1 && !isCollidableTile(layer->getTileGIDAt(cocos2d::Vec2(column + 1, row + 1)))) { flags |= RIGHT; }
            }
            
            collider->setFlags(flags);
            
            _colliders.push_back(collider);
        }
    }
}

bool Level::isCollidableTile(uint32_t gid) const {
    return gid == 35 || gid == 36 || gid == 37 || gid == 38 || gid == 39;
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

