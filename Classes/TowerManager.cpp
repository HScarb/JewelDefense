// TowerManager.cpp
#include "TowerManager.h"
#include "TowerNew.h"
USING_NS_CC;

TowerManager::TowerManager()
{
	m_towerList.clear();
}

TowerManager::~TowerManager()
{
}

TowerManager* TowerManager::create()
{
	TowerManager * towerMgr = new TowerManager;
	if (towerMgr && towerMgr->init())
	{
		towerMgr->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(towerMgr);
	}
	return towerMgr;
}

bool TowerManager::init()
{
	if (!Node::init())
		return false;

	// add touch listener
	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [&](Touch * touch, Event * event)
	{
		Vec2 pos = touch->getLocation();

		// get tower chlicked
		Tower * clickedTower = findClickedTower(pos);
		if (clickedTower == nullptr)
			return false;
		else
			clickedTower->showTowerOprBtns();

		return true;
	};
	listener->onTouchEnded = [](Touch *touch, Event *event)
	{

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void TowerManager::buildOnPosition(Vec2 pos)
{

}

void TowerManager::towerLogic(float dt)
{
//	for (auto tower : m_towerList)
//	{
//		tower->checkAtkMonster(dt);
//	}
}

void TowerManager::addToTowerList(Tower* tower)
{
	m_towerList.pushBack(tower);
}

Tower* TowerManager::findClickedTower(Vec2 pos)
{
	for (auto tower : m_towerList)
	{
		if (tower->isClickMe(pos))
		{
			return tower;
		}
	}
	return nullptr;
}