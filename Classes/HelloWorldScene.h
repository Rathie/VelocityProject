#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "MainMenuScene.h"

#include "InstructionsScene.h"
#include "SplashScene.h"
#include "GameOverScene.h"
#include "Definitions.h"
class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
 bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	 void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	 void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	 void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
	 void onAcceleration(cocos2d::Acceleration* accel, cocos2d::Event* event);
	 void update(float);
	 void UpdateTimer(float dt);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
	void PlayerDeath();
	void PlayerMovement();
	bool CollisionDetection(cocos2d::CCSprite* sprite, cocos2d::CCSprite* targetSprite);
	void SpaceObjectMovement();
	void StarMovement();
	void SpaceObjectPlacement();
};

#endif // __HELLOWORLD_SCENE_H__
