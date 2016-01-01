// Bullet.cpp
#include "Bullet.h"
#include "GlobalConsts.h"
#include "GlobalPath.h"
#include "Monster.h"
USING_NS_CC;

Bullet::Bullet()
{
	m_isUsing = false;
	m_target = nullptr;
	m_iSpeed = 1;
	m_iDamage = 0;
	m_bulletType = en_NoneBullet;
}

Bullet::~Bullet()
{
}

void Bullet::lockTarget(Monster* monster)
{
	if(monster != nullptr && monster->getSprite() != nullptr)
	{
		m_target = monster;
		onLockTarget(m_target);
	}
}

Monster* Bullet::getTarget()
{
	return this->m_target;
}

void Bullet::setIsUsing(bool flag)
{
	this->m_isUsing = flag;
}

bool Bullet::getIsUsing()
{
	return m_isUsing;
}

bool Bullet::isReached()
{
	return m_isReached;
}

void Bullet::turnToTarget()
{
	Vec2 shootVector = this->getTarget()->getPosition() - this->convertToWorldSpace(this->getPosition());
	float shootAngle = ccpToAngle(shootVector);
	float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);

	this->setRotation(cocosAngle);
}

// Real Bullets
MachineGunBullet::MachineGunBullet()
{
	m_iSpeed = BULLETSPEED_MACHINEGUN;
	m_bulletType = en_MachineGunBullet;
}

MachineGunBullet::~MachineGunBullet()
{
}

bool MachineGunBullet::init()
{
	bool bRet = false;

	do
	{
		Sprite * sprite = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey(PATH_BULLET_MATHINEGUN));
		CC_BREAK_IF(!sprite);
		bindSprite(sprite);

		bRet = true;
	} while (0);
	
	return bRet;
}

void MachineGunBullet::onLockTarget(Entity* target)
{
	this->setVisible(true);
	m_isReached = false;
	/*
	因为我们的子弹是添加到子弹管理器的，所以子弹的坐标是相对于子弹管理器的
	假设子弹管理器在屏幕200,120的位置，如果设子弹坐标为10,10，那么实际上子弹就在210,130的位置
	于是子弹移动目的地不能单纯地设为怪物的坐标，因为我们不能保证怪物的坐标不是相对坐标
	要进行下面几个步骤
	1.将怪物坐标转换为相对屏幕的坐标，也就是世界坐标
	2.将怪物的世界坐标转换到子弹的父类（子弹管理器）的相对坐标
	3.总结：将怪物的坐标转换为和子弹同一层次的坐标:
	-convertToWorldSpace函数就是把某个坐标转换到世界坐标
	-convertToNodeSpace 函数就是把某个坐标转换到相对于某个节点的坐标
	这两种转换都要基于对象的父节点来进行转换，于是在onLockAim函数中，aimWorldPos变量就是aim实体相对于屏幕的坐标
	dstPos就是相对于子弹管理器的坐标
	*/
	Vec2 aimWorldPos = target->getParent()->convertToWorldSpace(target->getPosition());
	Vec2 dstPos = this->getParent()->convertToNodeSpace(aimWorldPos);
	Vec2 towerPos = this->getPosition();
	float duration = ccpDistance(dstPos, towerPos) / BULLETSPEED_MACHINEGUN;
	auto moveTo = MoveTo::create(duration, dstPos);
	// when move end, call moveEnd function
	auto callFunc = CallFunc::create([&]() {
		moveEnd();
	});

	auto actions = Sequence::create(moveTo, callFunc, NULL);
	this->runAction(actions);
}

void MachineGunBullet::moveEnd()
{
	this->setVisible(false);
	m_isReached = true;
}

FreezeArrowBullet::FreezeArrowBullet()
{
	m_iSpeed = BULLETSPEED_FREEZEARROW;
	m_bulletType = en_FreezeArrowBullet;
}

FreezeArrowBullet::~FreezeArrowBullet()
{
}

bool FreezeArrowBullet::init()
{
	bool bRet = false;
	do
	{
		Sprite * sprite = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey(PATH_BULLET_FREEZEARROW));
		CC_BREAK_IF(!sprite);
		this->bindSprite(sprite);

		bRet = true;
	} while (0);
	return bRet;
}

void FreezeArrowBullet::onLockTarget(Entity* target)
{
	this->setVisible(true);
	m_isReached = false;

	Vec2 aimWorldPos = target->getParent()->convertToWorldSpace(target->getPosition());
	Vec2 dstPos = this->getParent()->convertToNodeSpace(aimWorldPos);
	Vec2 towerPos = this->getPosition();
	float duration = ccpDistance(dstPos, towerPos) / BULLETSPEED_FREEZEARROW;
	auto moveTo = MoveTo::create(duration, dstPos);
	// when move end, call moveEnd function
	auto callFunc = CallFunc::create([&]() {
		moveEnd();
	});

	auto actions = Sequence::create(moveTo, callFunc, NULL);
	this->runAction(actions);
}

void FreezeArrowBullet::moveEnd()
{
	this->setVisible(false);
	m_isReached = true;
}

CannonRocketBullet::CannonRocketBullet()
{
	m_iSpeed = BULLETSPEED_CANNONROCKET;
	m_bulletType = en_RocketBullet;
}

CannonRocketBullet::~CannonRocketBullet()
{
}

bool CannonRocketBullet::init()
{
	bool bRet = false;

	do
	{
		Sprite * sprite = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey(PATH_BULLET_ROCKET));
		CC_BREAK_IF(!sprite);

		bindSprite(sprite);

		bRet = true;
	} while (0);
	return bRet;
}

void CannonRocketBullet::onLockTarget(Entity* target)
{
	this->setVisible(true);
	m_isReached = false;

	Vec2 aimWorldPos = target->getParent()->convertToWorldSpace(target->getPosition());
	Vec2 dstPos = this->getParent()->convertToNodeSpace(aimWorldPos);
	Vec2 towerPos = this->getPosition();
	float duration = ccpDistance(dstPos, towerPos) / BULLETSPEED_CANNONROCKET;
	auto moveTo = MoveTo::create(duration, dstPos);
	// when move end, call moveEnd function
	auto callFunc = CallFunc::create([&]() {
		moveEnd();
	});

	auto actions = Sequence::create(moveTo, callFunc, NULL);
	this->runAction(actions);
}

void CannonRocketBullet::moveEnd()
{
	this->setVisible(false);
	m_isReached = true;
}