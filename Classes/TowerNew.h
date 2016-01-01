// TowerNew.h
#pragma once
#include "cocos2d.h"
#include "Entity.h"
#include "ui\CocosGUI.h"
class Monster;

class Tower : public Entity
{
public:
	Tower();
	~Tower();
	CREATE_FUNC(Tower);
	virtual bool init();

	static Tower * createWithSprite(cocos2d::Sprite * sprite);
	bool initWithSprite(cocos2d::Sprite * sprite);

	static Tower *createFromCsvFileByID(int heroID);
	bool initFromCsvFileByID(int heroID);

	void upgrade();

	bool isClickMe(cocos2d::Vec2 pos);
	// -- BTN --
public:
	void showTowerOprBtns();
	void deleteOprBtns();
private:
	cocos2d::ui::Button* m_cancelBtn;
	cocos2d::ui::Button* m_deleteBtn;
	cocos2d::ui::Button* m_upgradeBtn;

	bool m_isOprBtnsShow;

	void resetOprBtns();
	void cancelClick(Ref* target, cocos2d::ui::TouchEventType type);
	void deleteClick(Ref* target, cocos2d::ui::TouchEventType type);
	void upgradeClick(Ref* target, cocos2d::ui::TouchEventType type);

	void removeBtns();

	// --properties--
	CC_SYNTHESIZE(int, m_baseAtk, BaseAtk);
	CC_SYNTHESIZE(int, m_curAtk, CurAtk);
	CC_SYNTHESIZE(int, m_atkSpeed, AtkSpeed);
	CC_SYNTHESIZE(int, m_atkRange, AtkRange);
	CC_SYNTHESIZE(int, m_upgradeCostBase, UpgradeCostBase);
	CC_SYNTHESIZE(float, m_upgradeAtkMultiple, UpgradeAtkMultiple);

	CC_SYNTHESIZE(cocos2d::Vec2, m_tileCoord, TileCoord);
	void setTarget(Monster * target) { m_atkMonster = target; }
	Monster * getTarget() { return m_atkMonster; }
	// -- Hero Methods --

protected:
	void towerLogic(float dt);

	virtual Monster * getClosestTarget();

	void turnToTarget();

	Monster * m_atkMonster;
};

class MachineGunTower : public Tower
{
public:
	MachineGunTower();
	~MachineGunTower();
	bool init();
	CREATE_FUNC(MachineGunTower);

	void fire(float dt);
};

class FreezeTower : public Tower
{
public:
	FreezeTower();
	~FreezeTower();
	bool init();
	CREATE_FUNC(FreezeTower);

	void fire(float dt);
};

class CannonTower : public Tower
{
public:
	CannonTower();
	~CannonTower();
	bool init();
	CREATE_FUNC(CannonTower);

	void fire(float dt);

	Monster * getClosestTarget();
};