// Projectile.cpp
#include "Projectile.h"
#include "GameMediator.h"
#include "GlobalPath.h"
#include "Monster.h"
USING_NS_CC;

bool Projectile::initWithFile(const char* filename)
{
	m_sprite = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey(filename));
	this->addChild(m_sprite);

	m_iSpeed = 0;
	m_targetPos = Vec2::ZERO;
	m_iDamage = 0;

	gm = GameMediator::getInstance();

	hasRemoved = false;

	scheduleUpdate();

	return true;
}

cocos2d::Rect Projectile::getRect()
{
	Rect rect = Rect(this->getPosition().x - m_sprite->getContentSize().width * 0.5f,
		this->getPosition().y - m_sprite->getContentSize().height* 0.5f,
		m_sprite->getContentSize().width,
		m_sprite->getContentSize().height);

	return rect;
}

void Projectile::removeSelf()
{
	if (hasRemoved)
		return;

	hasRemoved = true;

	unscheduleAllSelectors();
	this->stopAllActions();
	this->removeFromParentAndCleanup(true);
}

/////////////////////////////

MachineGunProjectile* MachineGunProjectile::createWithTargetPos(cocos2d::Vec2 pos)
{
	MachineGunProjectile * m = new MachineGunProjectile;
	if(m && m->initWithTargetPos(pos))
	{
		m->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(m);
	}
	return m;
}

bool MachineGunProjectile::initWithTargetPos(cocos2d::Vec2 pos)
{
	if (!Projectile::initWithFile(PATH_BULLET_MATHINEGUN))
		return false;

	this->setTargetPos(pos);

	moveToTargetPos();

	return true;
}

void MachineGunProjectile::update(float delta)
{
	for(auto temp : gm->m_monsterList)
	{
		if(this->getRect().intersectsRect(temp->getRect()) && temp->getHP() > 0)
		{
			temp->setHP(temp->getHP() - this->getDamage());
			removeSelf();
			break;
		}
	}
}

void MachineGunProjectile::moveToTargetPos()
{
	float distance = ccpDistance(this->getPosition(), this->getTargetPos());
	float moveDur = distance / (float)m_iSpeed;
	auto moveTo = MoveTo::create(moveDur, this->getTargetPos());
	auto moveDone = CallFunc::create(CC_CALLBACK_0(MachineGunProjectile::removeSelf, this));
	this->runAction(Sequence::create(moveTo, moveDone, nullptr));
}

//// Freeze
FreezeProjectile* FreezeProjectile::createWithTargetPos(cocos2d::Vec2 pos)
{
	FreezeProjectile * m = new FreezeProjectile;
	if (m && m->initWithTargetPos(pos))
	{
		m->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(m);
	}
	return m;
}

bool FreezeProjectile::initWithTargetPos(cocos2d::Vec2 pos)
{
	if (!Projectile::initWithFile(PATH_BULLET_FREEZEARROW))
		return false;

	this->setTargetPos(pos);

	moveToTargetPos();

	return true;
}

void FreezeProjectile::update(float delta)
{
	for (auto temp : gm->m_monsterList)
	{
		if (this->getRect().intersectsRect(temp->getRect()) && temp->getHP() > 0)
		{
//			temp->changeSpeed();
			temp->setHP(temp->getHP() - this->getDamage());
			removeSelf();
			break;
		}
	}
}

void FreezeProjectile::moveToTargetPos()
{
	float distance = ccpDistance(this->getPosition(), this->getTargetPos());
	float moveDur = distance / (float)m_iSpeed;
	auto moveTo = MoveTo::create(moveDur, this->getTargetPos());
	auto moveDone = CallFunc::create(CC_CALLBACK_0(FreezeProjectile::removeSelf, this));
	this->runAction(Sequence::create(moveTo, moveDone, nullptr));
}

/// cannon rocket
CannonProjectile* CannonProjectile::createWithTarget(Monster * monster)
{
	CannonProjectile * m = new CannonProjectile;
	if (m && m->initWithTarget(monster))
	{
		m->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(m);
	}
	return m;
}

bool CannonProjectile::initWithTarget(Monster * monster)
{
	if (!Projectile::initWithFile(PATH_BULLET_ROCKET))
		return false;

	m_target = monster;
	angularVelocity = 5.0f;

	return true;
}

void CannonProjectile::update(float delta)
{
	Vec2 targetPos = m_target->getPosition();
	Vec2 myPos = this->getPosition();

	if (gm->getMapLayer()->isOutOfMap(targetPos) || m_target->getHP() <= 0) {
		removeSelf();
	}

	if (this->getRect().intersectsRect(m_target->getRect()) && m_target->getHP() > 0)
	{
		m_target->setHP(m_target->getHP() - this->getDamage());

		removeSelf();
	}

	float radian = atan2f(targetPos.y - myPos.y, targetPos.x - myPos.x);
	float angle = -CC_RADIANS_TO_DEGREES(radian);
	angle = to360Angle(angle);

	float myAngle = to360Angle(this->getRotation());
	float tempAngle;
	if (myAngle < angle) 
	{
		tempAngle = angularVelocity;
	}
	else if (myAngle >angle) 
	{
		tempAngle = -angularVelocity;
	}
	else 
	{
		tempAngle = 0;
	}

	if (360 - abs(angle - myAngle) < abs(angle - myAngle))
	{
		tempAngle *= -1;
	}

	this->setRotation(this->getRotation() + tempAngle);
	float myRadian = CC_DEGREES_TO_RADIANS(this->getRotation());

	float x = cosf(myRadian) * this->getSpeed() * delta;
	float y = -sinf(myRadian) * this->getSpeed() * delta;

	this->setPosition(ccp(myPos.x + x, myPos.y + y));
}