#pragma once

#include "cocos2d.h"
#include "GamePad.h"

#define NUM_PLAYERS 2
class Level;
class Character;
class PickupableInterface;
namespace cocos2d { namespace ui { class Layout; } }

class LevelScene : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void update(float dt);
    
private:
    CREATE_FUNC(LevelScene);
    
    void initSpriteSheets();
    void initAnimations();
    void collisionDetection();
    void _drawStatusBar();
    void _drawBackground();
  
    bool _paused = false;
    
    // Items & Characters
    Character* _characters[NUM_PLAYERS];
    std::vector<PickupableInterface*> _pickups;
    
    // Map
    Level* _map;
    
    cocos2d::Label* _pauseLabel;
    ControllerState _previousControllerState[NUM_PLAYERS];
};
