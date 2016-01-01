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
	��Ϊ���ǵ��ӵ�����ӵ��ӵ��������ģ������ӵ���������������ӵ���������
	�����ӵ�����������Ļ200,120��λ�ã�������ӵ�����Ϊ10,10����ôʵ�����ӵ�����210,130��λ��
	�����ӵ��ƶ�Ŀ�ĵز��ܵ�������Ϊ��������꣬��Ϊ���ǲ��ܱ�֤��������겻���������
	Ҫ�������漸������
	1.����������ת��Ϊ�����Ļ�����꣬Ҳ������������
	2.���������������ת�����ӵ��ĸ��ࣨ�ӵ������������������
	3.�ܽ᣺�����������ת��Ϊ���ӵ�ͬһ��ε�����:
	-convertToWorldSpace�������ǰ�ĳ������ת������������
	-convertToNodeSpace �������ǰ�ĳ������ת���������ĳ���ڵ������
	������ת����Ҫ���ڶ���ĸ��ڵ�������ת����������onLockAim�����У�aimWorldPos��������aimʵ���������Ļ������
	dstPos����������ӵ�������������
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