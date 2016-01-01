// TowerNew.cpp
#include "TowerNew.h"
#include "Monster.h"
#include "EnumTDProp.h"
#include "CsvUtil.h"
#include "GlobalPath.h"
#include "GameMediator.h"
#include "Projectile.h"

USING_NS_CC;
using namespace ui;

Tower::Tower()
{
	m_iLevel = 1;
	m_cancelBtn = NULL;
	m_deleteBtn = NULL;
	m_upgradeBtn = NULL;
	m_isOprBtnsShow = false;
	m_atkMonster = nullptr;
}
Tower::~Tower()
{
}

bool Tower::init()
{
	return true;
}

Tower* Tower::createWithSprite(Sprite* sprite)
{
	Tower* tower = new Tower();
	if (tower && tower->initWithSprite(sprite))
	{
		tower->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(tower);
	}
	return tower;
}

bool Tower::initWithSprite(Sprite* sprite)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!sprite);
		bindSprite(sprite);

		// create bullet manager

		bRet = true;
	} while (0);

	return bRet;
}

Tower* Tower::createFromCsvFileByID(int heroID)
{
	Tower* hero = new Tower();
	if (hero && hero->initFromCsvFileByID(heroID))
	{
		hero->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(hero);
	}
	return hero;
}

bool Tower::initFromCsvFileByID(int heroID)
{
	bool bRet = false;

	do
	{
		CsvUtil* csvUtil = CsvUtil::getInstance();

		std::string sHeroID = StringUtils::toString(heroID);

		// find ID line
		int iLine = csvUtil->findValueInWithLine(sHeroID.c_str(), enTowerPropConf_ID, PATH_CSV_TOWER);

		CC_BREAK_IF(iLine < 0);

		setID(heroID);
		setModelID(csvUtil->getInt(iLine, enTowerPropConf_ModelID, PATH_CSV_TOWER));
		setBaseAtk(csvUtil->getInt(iLine, enTowerPropConf_BaseAtk, PATH_CSV_TOWER));
		setCurAtk(getBaseAtk());
		setAtkSpeed(csvUtil->getInt(iLine, enTowerPropConf_AtkSpeed, PATH_CSV_TOWER));
		setAtkRange(csvUtil->getInt(iLine, enTowerPropConf_AtkRange, PATH_CSV_TOWER));
		setUpgradeCostBase(csvUtil->getInt(iLine, enTowerPropConf_UpgradeCostBase, PATH_CSV_TOWER));
		setUpgradeAtkMultiple(csvUtil->getFloat(iLine, enTowerPropConf_UpgradeAtkBase, PATH_CSV_TOWER));

		Sprite* sprite = Sprite::create(StringUtils::format("sprite/tower/tower_%d.png", heroID).c_str());
		CC_BREAK_IF(!sprite);
		CC_BREAK_IF(!initWithSprite(sprite));

		// schedule
		this->schedule(schedule_selector(Tower::towerLogic), 0.2f);

		bRet = true;
	} while (0);
	return bRet;
}

void Tower::upgrade()
{
	if (getSprite() != NULL)
	{
		getSprite()->stopAllActions();
	}

	std::string sFilePath = StringUtils::format("sprite/hero/border_%d.png", m_iLevel);
	Sprite* sprite = Sprite::create(sFilePath.c_str());

	bindSprite(sprite);

	m_iLevel++;

	if (m_iLevel == 2)
	{
		auto rotateBy = RotateBy::create(25.0f, 360, 360);
		auto repeat = RepeatForever::create(rotateBy);

		sFilePath = StringUtils::format("sprite/hero/magic_border_%d.png", m_iLevel);
		sprite = Sprite::create(sFilePath.c_str());
		sprite->setOpacity(180);
		sprite->runAction(repeat);
		this->addChild(sprite, 10);
	}
}

// -- bind hero -- 
bool Tower::isClickMe(Point pos)
{
	Size size = getSprite()->getContentSize();
	Point borderPos = getPosition();

	Point srcPos = Point(borderPos.x - size.width / 2, borderPos.y + size.height / 2);
	Point destPos = Point(borderPos.x + size.width / 2, borderPos.y - size.height / 2);

	if (pos.x >= srcPos.x && pos.x <= destPos.x && pos.y <= srcPos.y && pos.y >= destPos.y) {
		return true;
	}

	return false;
}

void Tower::showTowerOprBtns()
{
	if (m_isOprBtnsShow == true)
	{
		return;
	}
	if (m_cancelBtn == NULL)
	{
		// create an button
		auto heroOprBtn = Button::create();
		heroOprBtn->loadTextureNormal("button2.png");

		// 用clone函数复制三个按钮对象
		m_cancelBtn = (Button*)heroOprBtn->clone();
		m_deleteBtn = (Button*)heroOprBtn->clone();
		m_upgradeBtn = (Button*)heroOprBtn->clone();

		// init button position
		resetOprBtns();

		m_cancelBtn->setTitleText("cancel");
		m_deleteBtn->setTitleText("delete");
		m_upgradeBtn->setTitleText("upgrade");

		m_cancelBtn->addTouchEventListener(this, toucheventselector(Tower::cancelClick));
		m_deleteBtn->addTouchEventListener(this, toucheventselector(Tower::deleteClick));
		m_upgradeBtn->addTouchEventListener(this, toucheventselector(Tower::upgradeClick));

		this->addChild(m_cancelBtn, 999);
		this->addChild(m_deleteBtn, 999);
		this->addChild(m_upgradeBtn, 999);
	}

	m_cancelBtn->setEnabled(true);
	m_deleteBtn->setEnabled(true);
	m_upgradeBtn->setEnabled(true);

	m_cancelBtn->setVisible(true);
	m_deleteBtn->setVisible(true);
	m_upgradeBtn->setVisible(true);

	// add special effect to button
	m_cancelBtn->runAction(EaseBounceOut::create(MoveBy::create(0.5f, Point(0, 100))));
	m_deleteBtn->runAction(EaseBounceOut::create(MoveBy::create(0.5f, Point(-100, 0))));
	m_upgradeBtn->runAction(EaseBounceOut::create(MoveBy::create(0.5f, Point(100, 0))));

	m_isOprBtnsShow = true;
}

void Tower::deleteOprBtns() {
	if (m_cancelBtn != NULL) {
		m_cancelBtn->setEnabled(false);
		m_deleteBtn->setEnabled(false);
		m_upgradeBtn->setEnabled(false);
		// my add
		m_cancelBtn->setVisible(false);
		m_deleteBtn->setVisible(false);
		m_upgradeBtn->setVisible(false);

		resetOprBtns();
	}

	m_isOprBtnsShow = false;
}

void Tower::resetOprBtns()
{
	// reset button to mid
	Size size = this->getContentSize();
	Point pos = Point(0, 0);
	m_cancelBtn->setPosition(pos);
	m_deleteBtn->setPosition(pos);
	m_upgradeBtn->setPosition(pos);
}

void Tower::cancelClick(Ref* target, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		deleteOprBtns();
	}
}

void Tower::deleteClick(Ref* target, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		// this->delete();
		// m_hero = NULL;

		deleteOprBtns();
	}
}

void Tower::upgradeClick(Ref* target, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		this->upgrade();
		deleteOprBtns();
	}
}

void Tower::removeBtns()
{
	m_cancelBtn->setEnabled(false);
	m_deleteBtn->setEnabled(false);
	m_upgradeBtn->setEnabled(false);
}

Monster* Tower::getClosestTarget()
{
	Monster* closestTarget = nullptr;
	double maxDistant = 99999;

	GameMediator * m = GameMediator::getInstance();
	for(auto monster : m->m_monsterList)
	{
		if (monster->getHP() <= 0)
			continue;
		double curDistance = ccpDistance(this->getPosition(), monster->getPosition());
		if(curDistance < maxDistant)
		{
			closestTarget = monster;
			maxDistant = curDistance;
		}
	}

	if(maxDistant < this->getAtkRange())
	{
		return closestTarget;
	}
	return nullptr;
}

void Tower::turnToTarget()
{
	// turn to target
	Vec2 shootVector = m_atkMonster->getPosition() - this->getPosition();
	float shootAngle = ccpToAngle(shootVector);
	float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);

	float rotateSpeed = (float)(0.25 / M_PI);
	//	float rotateDuration = fabs(shootAngle * rotateSpeed);
	//	this->getSprite()->runAction(RotateTo::create(rotateDuration, cocosAngle));
	this->getSprite()->setRotation(cocosAngle);
}

void Tower::towerLogic(float dt)
{
	if(this->m_atkMonster == nullptr)
	{
		this->setTarget(this->getClosestTarget());
	}
	else
	{
		double curDistance = ccpDistance(this->getPosition(), this->getTarget()->getPosition());
		if(this->getTarget()->getHP() <= 0 || curDistance > this->getAtkRange())
		{
			this->setTarget(this->getClosestTarget());
		}
	}

	if(this->getTarget() != nullptr)
	{
		turnToTarget();
	}
}

// -------------------------- Machine Gun Tower
MachineGunTower::MachineGunTower()
{
}

MachineGunTower::~MachineGunTower()
{
}

bool MachineGunTower::init()
{
	if (!Tower::initFromCsvFileByID(1))
		return false;

	schedule(schedule_selector(MachineGunTower::fire), this->getAtkSpeed());

	return true;
}

void MachineGunTower::fire(float dt)
{
	if(this->getTarget() != nullptr)
	{
		GameMediator * m = GameMediator::getInstance();

		Vec2 shootVector = this->getTarget()->getPosition() - this->getPosition();
		Vec2 normalizedShootVector = shootVector.getNormalized();
		Vec2 overshotVector = normalizedShootVector * this->getAtkRange();
		Vec2 offscreenPoint = this->getPosition() + overshotVector;

		auto projectile = MachineGunProjectile::createWithTargetPos(offscreenPoint);
		projectile->setPosition(this->getPosition());
		m->getMapLayer()->addChild(projectile);
	}
}

// ---------------------------- FreezeTower ---------------------------
FreezeTower::FreezeTower()
{
}

FreezeTower::~FreezeTower()
{
}

bool FreezeTower::init()
{
	if (!Tower::initFromCsvFileByID(2))
		return false;

	return true;
}

void FreezeTower::fire(float dt)
{
	if (this->getTarget() != nullptr)
	{
		GameMediator * m = GameMediator::getInstance();

		Vec2 shootVector = this->getTarget()->getPosition() - this->getPosition();
		Vec2 normalizedShootVector = shootVector.getNormalized();
		Vec2 overshotVector = normalizedShootVector * this->getAtkRange();
		Vec2 offscreenPoint = this->getPosition() + overshotVector;

		auto projectile = FreezeProjectile::createWithTargetPos(offscreenPoint);
		projectile->setPosition(this->getPosition());
		m->getMapLayer()->addChild(projectile);
	}
}

// ------------------------------ CannonTower ------------------------------
CannonTower::CannonTower()
{
}

CannonTower::~CannonTower()
{
}

bool CannonTower::init()
{
	if (!Tower::initFromCsvFileByID(3))
		return false;

	return true;
}

void CannonTower::fire(float dt)
{
	if (this->getTarget() != nullptr)
	{
		GameMediator * m = GameMediator::getInstance();

		CannonProjectile * projectile = CannonProjectile::createWithTarget(this->getTarget());
		projectile->setPosition(this->getPosition());
		projectile->setRotation(this->getRotation());
		m->getMapLayer()->addChild(projectile);
	}
}

Monster* CannonTower::getClosestTarget()
{
	Monster * farthestMonster = nullptr;

	GameMediator * m = GameMediator::getInstance();
	for(auto monster : m->m_monsterList)
	{
		if (monster->getHP() <= 0)
			continue;
		float curDistance = ccpDistance(this->getPosition(), monster->getPosition());
		if(curDistance < this->getAtkRange())
		{
			farthestMonster = monster;
			break;
		}
	}
	return farthestMonster;
}