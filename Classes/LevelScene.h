#pragma once

#include "cocos2d.h"
#include "GamePad.h"

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
    
    bool _paused = false;
    
    // Items & Characters
    Character* _character;
    std::vector<PickupableInterface*> _pickups;
    
    // Map
    Level* _map;
    
    cocos2d::Label* _pauseLabel;
    ControllerState _previousControllerState;
};
