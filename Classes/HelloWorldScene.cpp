#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <cstdlib>
#include <ctime>
#include "SimpleAudioEngine.h"  

USING_NS_CC;

using namespace cocostudio::timeline;
Sprite* Player;
Sprite* Planet;
Sprite* Stars[100];
Sprite* Bullet[500];
Sprite* Background;
bool bulletInPlay[500];
Sprite* spaceObjects[10];
Sprite* Background1;
Sprite* Backgrounds[10];
Label* score;
Label* playerHealth;
ParticleSystemQuad* spaceShipThruster;
int spaceObjectsHitCount[10];
float spaceObjectRotation[10];
bool velocityMode;
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
int tempScore;
std::string BGM[12];

Scene* HelloWorld::createScene()
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
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
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
	Background = (Sprite*)rootNode->getChildByName("Background");
	Player = (Sprite*)rootNode->getChildByName("Player");
	Planet = (Sprite*)rootNode->getChildByName("Planet");
	spaceShipThruster = (ParticleSystemQuad*)rootNode->getChildByName("ShipThrusters");
	score = Label::create();
	score->setColor(Color3B(255, 255, 255));
	score->setDimensions(250, 100 );
	score->setPosition(Vec2((screenSizeX - 200), (screenSizeY -300)));
	score->setScale(1.5f);
	score->setOpacity(255);

	playerHealth = Label::create();
	playerHealth->setColor(Color3B(255, 255, 255));
	playerHealth->setDimensions(250, 100);
	playerHealth->setPosition(Vec2((screenSizeX - 200), (screenSizeY - 300)));
	playerHealth->setScale(1.5f);
	playerHealth->setOpacity(255);
	Background1 = Sprite::create("star.png");


	this->addChild(Background1);

	this->addChild(Background);


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
		spaceObjectsHitCount[i] = 0;
		spaceObjectRotation[i] = rand() % 360;
		spaceObjects[i]->setScale(2.0f);
		leftSkew[i] = rand() % 3000 - 1500;
	}
	this->schedule(schedule_selector(HelloWorld::UpdateTimer), 0.5f);

	for (int i = 0; i < 100; i++)
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

	for (int i = 0; i < 500; i++)
	{
		if (i % 2)
		{
			Bullet[i] = Sprite::create("Bullet2.png");
		}
		else
		{
			Bullet[i] = Sprite::create("Bullet.png");
		}
		bulletInPlay[i] = false;
		Bullet[i]->setScale(0.5);
		this->addChild(Bullet[i]);
	}
	Player->setScale(1.1);
	this->addChild(spaceShipThruster);
	this->addChild(Player);
	this->addChild(Planet);
	this->addChild(score);
	this->addChild(playerHealth);

	hasBegun = true;
	velocityMode = false;
	movementSpeed = 2.125f;
	spaceObjectSpeed = 0.125f;
	starSpeed = 1.0f;
	health = 100;
	tempScore = 0;
	BGM[0] = "The Used - Paralyzed (Pro).mp3";
	BGM[1] = "Between The Buried And Me - Astral Body (Pro).mp3";
	BGM[2] = "Conquer Divide - Nightmares (Pro).mp3";
	BGM[3] = "Letlive - Muther (Pro).mp3";
	BGM[4] = "Nonpoint - Lights Camera Action (Pro).mp3";
	BGM[5] = "Nonpoint - What A Day (Pro).mp3";
	BGM[6] = "The Charm The Fury - The Virtue Of Leadership (Pro).mp3";
	BGM[7] = "The Used - Hospital (Pro).mp3";
	BGM[8] = "The Used - Pretty Handsome Awkward (Pro).mp3";
	BGM[9] = "Veil Of Maya - Ellie (Pro).mp3";
	BGM[10] = "Veil Of Maya - Mikasa (Pro) (1).mp3";
	BGM[11] = "Enter Shikari - The Jester (Pro).WAV";
	std::string str1 = BGM[rand() % 12];
	const char* string1 = str1.c_str();


	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
		string1, true);
    return true;
}
void HelloWorld::UpdateTimer(float dt)
{
	if (hasBegun == true)
	{
		if (spaceObjectSpeed < 125)
			spaceObjectSpeed += 0.125f;
		if (starSpeed < 150)
			starSpeed += 0.1f;
		if (movementSpeed < 125)
			movementSpeed += 0.125f;

		if (velocityMode == true)
		{
			Texture2D* temp;
			Texture2D* temp2;
					temp = Background->getTexture();
					temp2 = Background1->getTexture();
					if (temp != temp2)
					{
						Background->setTexture("star.png");
						if (movementSpeed < 125)
						movementSpeed + 1;
						if (spaceObjectSpeed < 125)
						spaceObjectSpeed + 1;
						for (int i = 0; i < 100; i++)
						{
							Stars[i]->setColor(Color3B(0, 0, 0));
						}
						for (int i = 0; i < 10; i++)
						{
							spaceObjects[i]->setColor(Color3B(0, 0, 0));
						}
						Player->setColor(Color3B(0, 0, 0));
						score->setColor(Color3B(0, 0, 0));
						playerHealth->setColor(Color3B(0, 0, 0));
						CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("scifi6.mp3");


					}
					else
					{
						Background->setTexture("1329766522.png.png");
						if (movementSpeed < 125)
							movementSpeed + 1;
						if (spaceObjectSpeed < 125)
							spaceObjectSpeed + 1;
						for (int i = 0; i < 100; i++)
						{
							Stars[i]->setColor(Color3B(255, 255, 255));
						}
						for (int i = 0; i < 10; i++)
						{
							spaceObjects[i]->setColor(Color3B(255, 255, 255));
						}
						Player->setColor(Color3B(255, 255, 255));
						score->setColor(Color3B(255, 255, 255));
						playerHealth->setColor(Color3B(255, 255, 255));
						CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("scifi6.mp3");

					}
		}


		if (Player->getColor() == Color3B(255.0f, 0.0f, 0.0f))
		{
			Player->setColor(Color3B(255, 255, 255));
			spaceShipThruster->setColor(Color3B(255.0f, 255, 255));

		}
		for (int i = 0; i < 10; i++)
		{
			if (spaceObjects[i]->getColor() == Color3B(255.0f, 0.0f, 0.0f))
			{
				spaceObjects[i]->setColor(Color3B(255, 255, 255));
			}
		}
		for (int i = 0; i < 250; i+=3)
		{
			Vec2 tempBullet = Player->getPosition();
			if (bulletInPlay[i] == false)
			{
				Bullet[i]->setPosition(tempBullet);
				bulletInPlay[i] = true;
			}
			else if (bulletInPlay[i] == true)
			{
				if (bulletInPlay[i + 1] == false)
				{
					Bullet[i + 1]->setPosition(tempBullet);
					bulletInPlay[i + 1] = true;
				}
			}
			else if (bulletInPlay[i+1] == true)
			{
				if (bulletInPlay[i + 2] == false)
				{
					Bullet[i + 2]->setPosition(tempBullet);
					bulletInPlay[i + 2] = true;
				}
			}
		}
	}
}
void HelloWorld::update(float delta)
{
	score->setPosition(Player->getPositionX() -50, (Player->getPositionY() + 50));
	playerHealth->setPosition(Player->getPositionX() + 250, (Player->getPositionY() + 50));

	if (hasBegun == true)
	{

		if (spaceObjectSpeed  >= 21)
		{
			velocityMode = true;
		}
		temp = Planet->getPosition();
		temp.y -= movementSpeed;
		Planet->setPosition(temp);
		if (hasBegun != false)
		{
			tempScore += movementSpeed;
			std::string temp = " ";
			std::ostringstream strs;
			strs << tempScore;
			std::string str = strs.str();
			if (velocityMode == true)
			{
				score->setString("VELOCITY OVERDRIVE : " + str);
			}
			else
			{
				score->setString("Velocity : " + str);
			}
		}
		std::ostringstream strs;
		strs << health;
		std::string str = strs.str();
		playerHealth->setString("Hull Integrity : " + str);
	

		for (int i = 0; i < 500; i++)
		{
			if (bulletInPlay[i] == true)
			{
				Vec2 bulletPos = Bullet[i]->getPosition();
				bulletPos.y += movementSpeed * 2;
				Bullet[i]->setPosition(bulletPos);

			}

			if (Bullet[i]->getPositionY() <= 0 || Bullet[i]->getPositionY() > screenSizeY)
			{
				bulletInPlay[i] = false;
			}

			if (bulletInPlay[i] == false)
			{
				Vec2 tempOutOfBounds = Vec2(Player->getPositionX(), -100);
				Bullet[i]->setPosition(tempOutOfBounds);
			}
		}

		for (int i = 0; i < 500; i++)
		{
			for (int f = 0; f < 10; f++)
			{
				if (CollisionDetection(Bullet[i], spaceObjects[f]))
				{
					spaceObjectsHitCount[f] = spaceObjectsHitCount[f] + 1;
					tempScore += 1;
					spaceObjects[f]->setColor(Color3B(255, 0, 0));
					bulletInPlay[i] = false;
				}
			}
		}

		for (int i = 0; i < 10; i++)
		{
			if(spaceObjectsHitCount[i] >= 1 )
			{
				spaceObjects[i]->setColor(Color3B(255, 255, 255));
				spaceObjects[i]->setPositionY(screenSizeY);
				spaceObjects[i]->setPositionX(rand() % screenSizeX);
				
				spaceObjectsHitCount[i] = 0;
			}
			spaceObjects[i]->setRotation(spaceObjectRotation[i]);
			spaceObjectRotation[i] += 1.25f;
				if (spaceObjectRotation[i] >= 360.0f)
				{
					spaceObjectRotation[i] = 0;
				}
			if (spaceObjects[i]->getPositionY() <= 0 || spaceObjects[i]->getPositionY() > screenSizeY)
			{
				spaceObjects[i]->setPositionY(screenSizeY);
				spaceObjects[i]->setPositionX(rand() % screenSizeX);
			}
			if (CollisionDetection(Player, spaceObjects[i]))
			{
				Player->setColor(Color3B(255.0f, 0.0f, 0.0f));
				spaceShipThruster->setColor(Color3B(255.0f, 0.0f, 0.0f));
				health = health - 1;
				spaceObjectsHitCount[i] += 20;
			}
		}
		StarMovement();
		SpaceObjectMovement();
		PlayerMovement();
		PlayerDeath();
	}

}

void HelloWorld::StarMovement()
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			Stars[i]->setRotation(spaceObjectRotation[j]);		
		}
		Vec2 starPos = Stars[i]->getPosition();
		starPos.y -= starSpeed;
		Stars[i]->setPosition(starPos);
		if (Stars[i]->getPositionY() <= 0 || Stars[i]->getPositionY() > screenSizeY)
		{
			Stars[i]->setPositionY(screenSizeY);
			Stars[i]->setPositionX(rand() % screenSizeX);

		}

	}
}

void HelloWorld::SpaceObjectMovement()
{
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
	}
}


bool HelloWorld::CollisionDetection(CCSprite* sprite, CCSprite* targetSprite)
{
	CCRect targetRect = targetSprite->boundingBox();
	CCRect rect = sprite->boundingBox();
	if (rect.intersectsRect(targetRect))
	{
		return true;
	}
	else
	{
		return false;
	}
}


void HelloWorld::PlayerMovement()
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

void HelloWorld::PlayerDeath()
{
	if (health <= 0)
	{
		std::string str1 = BGM[1];
		const char* string1 = str1.c_str();
		Player->setColor(Color3B(0, 0, 0));
		spaceShipThruster->setColor(Color3B(0, 0, 0));
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("scifi20.mp3");
		hasBegun = false;
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(string1, true);
		auto scene = GameOverScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
	}
}

bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	hasBegun = true;
	pos = touch->getLocation();

	return true;

}

void HelloWorld::onAcceleration(cocos2d::Acceleration* accel, cocos2d::Event* event)
{
	
	//pos.y += accel->y * movementSpeed;
	//pos.x += accel->x * movementSpeed;
}

void HelloWorld::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("scifi19.mp3");

}

void HelloWorld::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	pos = touch->getLocation();

}

void HelloWorld::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
