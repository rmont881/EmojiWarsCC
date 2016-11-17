#include "AppDelegate.h"
#include "Constants.h"
#include "GamePad.h"
#include "LevelScene.h"
#include "Menu.h"
#include "audio/include/SimpleAudioEngine.h"

USING_NS_CC;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

void AppDelegate::initGLContextAttrs() {
  GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
  GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages() { return 0; }

bool AppDelegate::applicationDidFinishLaunching() {
  // initialize director
  auto director = Director::getInstance();
  auto glview = director->getOpenGLView();
  if (!glview) {
    glview = GLViewImpl::createWithRect(
        GAME_NAME, cocos2d::Rect(0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT));
    director->setOpenGLView(glview);
  }

  // turn on display FPS
  director->setDisplayStats(true);
  director->setAnimationInterval(FIXED_TIME_STEP);
  glview->setDesignResolutionSize(RESOLUTION_WIDTH, RESOLUTION_HEIGHT,
                                  ResolutionPolicy::NO_BORDER);
  director->setContentScaleFactor(0.5f);

  register_all_packages();

  auto scene = LevelScene::createScene();
  director->runWithScene(scene);

  setupGamepad();

  return true;
}

// This function will be called when the app is inactive. Note, when receiving a
// phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
  Director::getInstance()->stopAnimation();

  // if you use SimpleAudioEngine, it must be paused
   CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
  Director::getInstance()->startAnimation();

  // if you use SimpleAudioEngine, it must resume here
   CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
