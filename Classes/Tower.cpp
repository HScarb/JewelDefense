// Tower.cpp
#include "Tower.h"
#include "CsvUtil.h"
#include "EnumTDProp.h"
#include "GlobalPath.h"
#include "ui\CocosGUI.h"
#include "GameMediator.h"
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
	m_isAtkCoolDown = false;
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
		m_bulletMgr = BulletManager::create();
		this->addChild(m_bulletMgr);

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

// Hero Methods
/*
*想办法获取关卡地图层里所有的出场怪物
m_atkMonster记录了英雄当前锁定的攻击目标
如果攻击目标为空，则用chooseAim函数从怪物列表中查找进入了攻击范围的怪物，设为攻击目标
如果攻击目标不为空，判断目标是否死亡，如果目标死亡，从怪物列表中删除这个怪物，并且将英雄攻击目标设置为空，本次攻击结束
如果攻击目标不为空，且英雄的攻击冷却结束，则调用atk函数进行攻击
判断攻击目标是否离开了英雄的攻击范围，调用checkAimIsOutOfRange函数进行判断，并且进行一些处理

*/
void Tower::checkAtkMonster(float ft)
{
	auto monsterList = GameMediator::getInstance()->m_monsterList;
	if (m_atkMonster != nullptr)
	{
		if (m_atkMonster->isDead())		// if the monster is dead
		{
			// delete it from monster list
//			monsterList.eraseObject(m_atkMonster);

			// clean monster's reference
			m_atkMonster = nullptr;
			return;
		}

		turnToTarget();

		// if weapon isn't cool down
  		if (m_isAtkCoolDown == false)
		{
			atk();
		}
		// judge if monster is out of range
		checkAimIsOutOfRange();
	}
	else
	{
		// choose one monster in range
		chooseAim();
	}
}

// 遍历怪物列表，判断一个怪物是否进入了英雄的攻击范围，如果是，则锁定攻击目标chooseAtkMonster
void Tower::chooseAim()
{
	auto monsterList = GameMediator::getInstance()->m_monsterList;
	for (auto monster : monsterList)
	{
		if (monster && !monster->isDead() && isInAtkRange(monster->getPosition()))
		{
			chooseAtkMonster(monster);
			log("InAtkRange!");
			break;
		}
	}
}

bool Tower::isInAtkRange(Point pos)
{
	int iDoubleAtkRange = getAtkRange();
	Point heroPos = getPosition();

	float length = pos.getDistanceSq(heroPos);		// 计算距离的平方，以免开更号出现误差
	if (length <= iDoubleAtkRange*iDoubleAtkRange)
	{
		return true;
	}
	return false;
}

void Tower::chooseAtkMonster(Monster* monster)
{
	m_atkMonster = monster;
}

void Tower::atk()
{
	// take out an unUsen bullet from bullet manager
	Bullet* bullet = m_bulletMgr->getBulletByType((BulletType)this->getID());
	if (bullet != NULL)
	{
		// 根据英雄情况设置子弹属性，锁定攻击目标
		Point heroWorldPos = this->getParent()->convertToWorldSpace(getPosition());	// 英雄位置相对于世界的坐标
		bullet->setPosition(bullet->getParent()->convertToNodeSpace(heroWorldPos));
		bullet->setDamage(getCurAtk());
		bullet->lockTarget(m_atkMonster);

		// mark attack cool down
		m_isAtkCoolDown = true;

		// 英雄有攻击间隔，指定若干秒后恢复攻击
		// 利用scheduleOnce函数在若干秒后调用atkCoolDownEnd
		this->scheduleOnce(schedule_selector(Tower::atkCoolDownEnd), getAtkSpeed() / 1000.0f);
	}
}

void Tower::atkCoolDownEnd(float dt)
{
	m_isAtkCoolDown = false;
}

void Tower::checkAimIsOutOfRange()
{
	auto monsterList = GameMediator::getInstance()->m_monsterList;
	if (m_atkMonster != nullptr)
	{
		if (isInAtkRange(m_atkMonster->getPosition()) == false)
		{
			missAtkMonster();
		}
	}
}

void Tower::missAtkMonster()
{
	log("out of range!");
	m_atkMonster = nullptr;
}

void Tower::turnToTarget()
{
	if (m_atkMonster)
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

	return true;
}

void MachineGunTower::atk()
{
	Bullet* bullet = m_bulletMgr->getBulletByType(en_MachineGunBullet);
	if (bullet != NULL)
	{
		// 根据英雄情况设置子弹属性，锁定攻击目标
		Point heroWorldPos = this->getParent()->convertToWorldSpace(getPosition());	// 英雄位置相对于世界的坐标
		bullet->setPosition(bullet->getParent()->convertToNodeSpace(heroWorldPos));
		bullet->setDamage(getCurAtk());
		bullet->lockTarget(m_atkMonster);
		bullet->turnToTarget();
		// mark attack cool down
		m_isAtkCoolDown = true;

		// 英雄有攻击间隔，指定若干秒后恢复攻击
		// 利用scheduleOnce函数在若干秒后调用atkCoolDownEnd
		this->scheduleOnce(schedule_selector(MachineGunTower::atkCoolDownEnd), getAtkSpeed() / 1000.0f);
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

void FreezeTower::atk()
{
	Bullet* bullet = m_bulletMgr->getBulletByType(en_FreezeArrowBullet);
	if (bullet != NULL)
	{
		// 根据英雄情况设置子弹属性，锁定攻击目标
		Point heroWorldPos = this->getParent()->convertToWorldSpace(getPosition());	// 英雄位置相对于世界的坐标
		bullet->setPosition(bullet->getParent()->convertToNodeSpace(heroWorldPos));
		bullet->setDamage(getCurAtk());
		bullet->lockTarget(m_atkMonster);
		bullet->turnToTarget();
		// mark attack cool down
		m_isAtkCoolDown = true;

		// 英雄有攻击间隔，指定若干秒后恢复攻击
		// 利用scheduleOnce函数在若干秒后调用atkCoolDownEnd
		this->scheduleOnce(schedule_selector(FreezeTower::atkCoolDownEnd), getAtkSpeed() / 1000.0f);
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

void CannonTower::atk()
{
	Bullet* bullet = m_bulletMgr->getBulletByType(en_RocketBullet);
	if (bullet != NULL)
	{
		// 根据英雄情况设置子弹属性，锁定攻击目标
		Point heroWorldPos = this->getParent()->convertToWorldSpace(getPosition());	// 英雄位置相对于世界的坐标
		bullet->setPosition(bullet->getParent()->convertToNodeSpace(heroWorldPos));
		bullet->setDamage(getCurAtk());
		bullet->lockTarget(m_atkMonster);
		bullet->turnToTarget();
		// mark attack cool down
		m_isAtkCoolDown = true;

		// 英雄有攻击间隔，指定若干秒后恢复攻击
		// 利用scheduleOnce函数在若干秒后调用atkCoolDownEnd
		this->scheduleOnce(schedule_selector(CannonTower::atkCoolDownEnd), getAtkSpeed() / 1000.0f);
	}
}