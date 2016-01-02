// TowerManager.h
#pragma once
#include "cocos2d.h"
#include "Tower.h"
class Tower;

class TowerManager : public cocos2d::Node
{
public:
	TowerManager();
	~TowerManager();
	static TowerManager * create();
	virtual bool init();

	void buildOnPosition(cocos2d::Vec2 pos);
	void towerLogic(float dt);
	void addToTowerList(Tower * tower);

public:
	cocos2d::Vector<Tower*> m_towerList;
private:
	Tower* findClickedTower(cocos2d::Vec2 pos);
};
