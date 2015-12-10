#include "SplashScene.h"
#include "MainMenuScene.h"
#include "Ball.h"
#include "Definitions.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Ball::Ball(cocos2d::Layer *layer)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	ballSprite = Sprite::create("ball.png");
	ballSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	auto ballBody = PhysicsBody::createCircle(ballSprite->getContentSize().width / 2);
	ballBody->setCollisionBitmask(BALL_COLLISION_BITMASK);
	ballBody->setContactTestBitmask(true);

	ballSprite->setPhysicsBody(ballBody);

	layer->addChild(ballSprite, 100);
}

