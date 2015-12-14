#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <cstdlib>
#include <ctime>
#include "SimpleAudioEngine.h"  

USING_NS_CC;

using namespace cocostudio::timeline;
Label* labelTouchInfo;
Sprite* Player;
Sprite* Planet;
Sprite* Stars[50];
Sprite* spaceObjects[10];
ParticleSystemQuad* spaceShipThruster;
ProgressTimer* velocity;
Label* score;


float movementSpeed;
float spaceObjectSpeed;
float starSpeed;
Vec2 pos;
Vec2 playerPos;
Vec2 temp;
bool hasBegun;
int screenSizeY;
int screenSizeX;
int health;
float leftSkew[10];
Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	srand(time(NULL));


	auto rootNode = CSLoader::createNode("MainScene.csb");

	addChild(rootNode);

	this->scheduleUpdate();
	cocos2d::Size frameSize = cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
	screenSizeY = frameSize.height;
	screenSizeX = frameSize.width;
	Device::setAccelerometerEnabled(true);




	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);

	auto AccelListener = EventListenerAcceleration::create(CC_CALLBACK_2(HelloWorld::onAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(AccelListener, this);

	score = (Label*)rootNode->getChildByName("Score");
	Player = (Sprite*)rootNode->getChildByName("Player");
	Planet = (Sprite*)rootNode->getChildByName("Planet");
	spaceShipThruster = (ParticleSystemQuad*)rootNode->getChildByName("ShipThrusters");
	for (int i = 0; i < 10; i++)
	{
		if (i % 2)
		{
			spaceObjects[i] = Sprite::create("Object1.png");
		}
		else
		{
			spaceObjects[i] = Sprite::create("Object2.png");
		}
		this->addChild(spaceObjects[i]);
		spaceObjects[i]->setPositionX(rand() % screenSizeX);
		spaceObjects[i]->setPositionY(rand() % screenSizeY);
		leftSkew[i] = rand() % 3000 - 1500;
	}
	this->schedule(schedule_selector(HelloWorld::UpdateTimer), 1.0f);

	for (int i = 0; i < 50; i++)
	{
		if (i % 2)
		{
			Stars[i] = Sprite::create("Untitled2.png");
		}
		else
		{
			Stars[i] = Sprite::create("Untitled.png");
		}
		this->addChild(Stars[i]);
		Stars[i]->setPositionX(rand() % screenSizeX);
		Stars[i]->setPositionY(rand() % screenSizeY);
	}
	this->addChild(Player);
	this->addChild(Planet);
	this->addChild(spaceShipThruster);
	this->addChild(score);
	hasBegun = false;
	movementSpeed = 2.125f;
	spaceObjectSpeed = 0.125f;
	starSpeed = 1.0f;
	health = 30;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
		"Enter Shikari - The Jester (Pro).WAV", true);
	return true;
}
void GameScene::UpdateTimer(float dt)
{
	if (spaceObjectSpeed < 85)
		spaceObjectSpeed += 0.225f;
	if (starSpeed < 100)
		starSpeed += 0.3f;
	if (movementSpeed < 60)
		movementSpeed += 0.125f;

	if (Player->getColor() == Color3B(255.0f, 0.0f, 0.0f))
	{
		Player->setColor(Color3B(255, 255, 255));
		spaceShipThruster->setColor(Color3B(255.0f, 255, 255));

	}
}
void GameScene::update(float delta)
{
	if (hasBegun == true)
	{
		playerPos = Player->getPosition();
		if (pos.x >= playerPos.x)
		{
			playerPos.x += movementSpeed;
		}
		if (pos.x <= playerPos.x)
		{
			playerPos.x -= movementSpeed;
		}
		if (pos.y >= playerPos.y)
		{
			playerPos.y += movementSpeed;
		}
		if (pos.y <= playerPos.y)
		{
			playerPos.y -= movementSpeed;
		}
		spaceShipThruster->setPositionX(Player->getPositionX());
		spaceShipThruster->setPositionY(Player->getPositionY() - 30);
		Player->setPosition(playerPos);
		temp = Planet->getPosition();
		temp.y -= movementSpeed;
		Planet->setPosition(temp);
		std::string temp = " ";
		float speed = movementSpeed;
		temp = std::to_string(speed);
		//score->setString(temp);
		for (int i = 0; i < 50; i++)
		{

			Vec2 starPos = Stars[i]->getPosition();
			starPos.y -= starSpeed;
			Stars[i]->setPosition(starPos);
			if (Stars[i]->getPositionY() <= 0 || Stars[i]->getPositionY() > screenSizeY)
			{
				Stars[i]->setPositionY(screenSizeY);
				Stars[i]->setPositionX(rand() % screenSizeX);

			}



		}


		for (int i = 0; i < 10; i++)
		{

			Vec2 spaceObjectPos = spaceObjects[i]->getPosition();
			spaceObjectPos.y -= spaceObjectSpeed;
			float rightSkew = 0;
			if (i == 0 || i == 2 || i == 4 || i == 6 || i == 8)
			{
				if (spaceObjects[i]->getPositionX() > leftSkew[i])
				{
					spaceObjectPos.x -= spaceObjectSpeed * 0.2f;
				}
				else if (spaceObjects[i]->getPositionX() < leftSkew[i])
				{
					spaceObjectPos.x += spaceObjectSpeed * 0.2f;
				}
			}

			spaceObjects[i]->setPosition(spaceObjectPos);

			if (spaceObjects[i]->getPositionY() <= 0 || spaceObjects[i]->getPositionY() > screenSizeY)
			{
				spaceObjects[i]->setPositionY(screenSizeY);
				spaceObjects[i]->setPositionX(rand() % screenSizeX);


			}


			CCSprite* targetSprite = spaceObjects[i];
			CCRect targetRect = targetSprite->boundingBox();
			CCRect playerRect = Player->boundingBox();
			if (playerRect.intersectsRect(targetRect))
			{
				Player->setColor(Color3B(255.0f, 0.0f, 0.0f));
				spaceShipThruster->setColor(Color3B(255.0f, 0.0f, 0.0f));
				health = health - 1;
			}
		}
		if (Player->getPositionX() > screenSizeX)
		{
			Player->setPositionX(0);
		}
		if (Player->getPositionX() < 0)
		{
			Player->setPositionX(screenSizeX);
		}
		if (Player->getPositionY() > screenSizeY)
		{
			Player->setPositionY(0);
		}
		if (Player->getPositionY() < 0)
		{
			Player->setPositionY(screenSizeY);
		}
	}
	if (health <= 0)
	{
		Player->setColor(Color3B(0, 0, 0));
		spaceShipThruster->setColor(Color3B(0, 0, 0));
		hasBegun = false;
	}
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	hasBegun = true;
	pos = touch->getLocation();

	return true;

}

void GameScene::onAcceleration(cocos2d::Acceleration* accel, cocos2d::Event* event)
{

	pos.x += accel->y;
	pos.y += accel->x;
}

void GameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{

}

void GameScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	pos = touch->getLocation();

}

void GameScene::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
