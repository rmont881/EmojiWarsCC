#include "Constants.h"
#include "AppDelegate.h"
#include "LevelScene.h""
#include "Menu.h"
#include "GamePad.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() {
}

void AppDelegate::initGLContextAttrs() {
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages() {
    return 0;
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithRect(GAME_NAME, cocos2d::Rect(0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT));
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);
    director->setAnimationInterval(FIXED_TIME_STEP);
    glview->setDesignResolutionSize(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, ResolutionPolicy::NO_BORDER);
    director->setContentScaleFactor(0.5f);

    register_all_packages();

    auto scene = MenuScene::createScene();
    director->runWithScene(scene);
    
    setupGamepad();

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
