// TDHUDLayer.cpp
#include "TDHUDLayer.h"
#include "GlobalConsts.h"
#include "GlobalPath.h"
#include "GameMediator.h"
#include "SceneManager.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace cocostudio::timeline;

TDHUDLayer * TDHUDLayer::m_TDHUDLayer = nullptr;

TDHUDLayer::TDHUDLayer()
{
	m_iLifeNum = 0;
	m_iMonsterNum = 0;
	m_iMoneyNum = 0;
	m_tapedTower = en_NoneTower;
}

TDHUDLayer::~TDHUDLayer()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool TDHUDLayer::init()
{
	if (!Layer::init())
		return false;

	auto UI = CSLoader::createNode("TD_Layer.csb");
	this->addChild(UI,LEVEL_TDHUDLAYER);

	// tower buttons
	m_machineGunTower = static_cast<ImageView*>(UI->getChildByName("Image_MachineGunTower"));
	m_freezeTower = static_cast<ImageView*>(UI->getChildByName("Image_FreezeTower"));
	m_cannonTower = static_cast<ImageView*>(UI->getChildByName("Image_CannonTower"));

	m_moneyLab = static_cast<Text*>(UI->getChildByName("moneyLab"));
	m_monsterLab = static_cast<Text*>(UI->getChildByName("monsterNumLab"));
	m_lifeLab = static_cast<Text*>(UI->getChildByName("lifeLab"));

	// subscribe touch event
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(TDHUDLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(TDHUDLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(TDHUDLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// subscribe notifications
	NotificationCenter::getInstance()->addObserver(this,
		callfuncO_selector(TDHUDLayer::recvRefreshMoneyNum),
		"MoneyChange",
		NULL);

	NotificationCenter::getInstance()->addObserver(this,
		callfuncO_selector(TDHUDLayer::recvRefreshMonsterNum),
		"MonsterNumChange",
		NULL);

	NotificationCenter::getInstance()->addObserver(this,
		callfuncO_selector(TDHUDLayer::recvRefreshLifeNum),
		"LifeChange",
		NULL);
	NotificationCenter::getInstance()->addObserver(this,
		callfuncO_selector(TDHUDLayer::recvAllMonsterDead),
		"AllMonsterDead",
		NULL);

	return true;
}

TDHUDLayer* TDHUDLayer::getInstance()
{
	if (m_TDHUDLayer == nullptr)
	{
		m_TDHUDLayer = new TDHUDLayer;
		if (m_TDHUDLayer && m_TDHUDLayer->init())
		{
			m_TDHUDLayer->autorelease();
			m_TDHUDLayer->retain();
		}
		else
		{
			CC_SAFE_DELETE(m_TDHUDLayer);
			m_TDHUDLayer = nullptr;
		}
	}
	return m_TDHUDLayer;
}

int TDHUDLayer::getiMoneyNum()
{
	return m_iMoneyNum;
}

int TDHUDLayer::getiLifeNum()
{
	return m_iLifeNum;
}

bool TDHUDLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	auto touchPos = touch->getLocation();

	if(m_machineGunTower->getBoundingBox().containsPoint(touchPos))
	{
		// check money
		if(1)		// have enough money
		{
			m_tapedTower = en_MachineGunTower;
			m_pendTower = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey(PATH_TOWER_MATHINEGUN));
			m_pendRange = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey(PATH_RANGE));
			m_pendTower->setPosition(touchPos);
			m_pendRange->setPosition(touchPos);
			this->addChild(m_pendTower, LEVEL_PENDTOWER);
			this->addChild(m_pendRange, LEVEL_PENDRANGE);
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (m_freezeTower->getBoundingBox().containsPoint(touchPos))
	{
		// check money
		if (1)		// have enough money
		{
			m_tapedTower = en_FreezeTower;
			m_pendTower = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey(PATH_TOWER_FREEZE));
			m_pendRange = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey(PATH_RANGE));
			m_pendTower->setPosition(touchPos);
			m_pendRange->setPosition(touchPos);
			this->addChild(m_pendTower, LEVEL_PENDTOWER);
			this->addChild(m_pendRange, LEVEL_PENDRANGE);
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (m_cannonTower->getBoundingBox().containsPoint(touchPos))
	{
		// check money
		if (1)		// have enough money
		{
			m_tapedTower = en_CannonTower;
			m_pendTower = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey(PATH_TOWER_CANNON));
			m_pendRange = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey(PATH_RANGE));
			m_pendTower->setPosition(touchPos);
			m_pendRange->setPosition(touchPos);
			this->addChild(m_pendTower, LEVEL_PENDTOWER);
			this->addChild(m_pendRange, LEVEL_PENDRANGE);
			return true;
		}
		else
		{
			return false;
		}
	}
	return true;
}

void TDHUDLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	auto touchPos = touch->getLocation();

	if (m_tapedTower == en_NoneTower)
		return;
	else
	{
		m_pendTower->setPosition(touchPos);
		m_pendRange->setPosition(touchPos);

		auto gm = GameMediator::getInstance();
		auto touchPosInMapLayer = gm->getMapLayer()->convertTouchToNodeSpace(touch);

		bool isBuildable = gm->getMapLayer()->canBuildOnPosition(touchPosInMapLayer);
		if(isBuildable)
		{
			m_pendTower->setOpacity(200);
		}
		else
		{
			m_pendTower->setOpacity(50);
		}
	}
}

void TDHUDLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	auto touchPos = touch->getLocation();
	auto gm = GameMediator::getInstance();

	if (m_tapedTower == en_NoneTower)
		return;
	else
	{
		this->removeChild(m_pendTower, true);
		m_pendTower = nullptr;
		this->removeChild(m_pendRange, true);
		m_pendRange = nullptr;
		gm->getMapLayer()->buildTowerOnPos(touchPos, m_tapedTower);
		m_tapedTower = en_NoneTower;
	}
}

void TDHUDLayer::recvRefreshMoneyNum(Ref* pData)
{
	int iAltValue = (int)pData;
	m_iMoneyNum += iAltValue;
	m_moneyLab->setText(StringUtils::toString(m_iMoneyNum));
}

void TDHUDLayer::recvRefreshMonsterNum(Ref* pData)
{
	int iAltValue = (int)pData;
	m_iMonsterNum += iAltValue;
	m_monsterLab->setText(StringUtils::toString(m_iMonsterNum));
}

void TDHUDLayer::recvRefreshLifeNum(Ref* pData)
{
	int iAltValue = (int)pData;
	m_iLifeNum += iAltValue;
	m_lifeLab->setText(StringUtils::toString(m_iLifeNum));

	if (m_iLifeNum <= 0) 
	{
		SceneManager::getInstance()->changeScene(en_LoseScene);
	}
}

void TDHUDLayer::recvAllMonsterDead(Ref* pData)
{
	if (m_iLifeNum > 0) 
	{
		SceneManager::getInstance()->changeScene(en_WinScene);
	}
}