// Bullet.h
#pragma once
#include "cocos2d.h"
#include "Entity.h"
#include "GlobalEnum.h"

class Monster;

class Bullet : public Entity
{
public:
	Bullet();
	~Bullet();

	// lock target
	void lockTarget(Monster * monster);

	Monster * getTarget();

	// if is using
	void setIsUsing(bool flag);
	bool getIsUsing();

	// if is reached target
	bool isReached();

	void turnToTarget();

protected:
	virtual void onLockTarget(Entity * target) = 0;

	bool m_isReached;
private:
	bool m_isUsing;

	Monster * m_target;

	CC_SYNTHESIZE(int, m_iDamage, Damage);
	CC_SYNTHESIZE(int, m_iSpeed, Speed);
	CC_SYNTHESIZE(BulletType, m_bulletType, BulletType);
};

class MachineGunBullet : public Bullet
{
public:
	MachineGunBullet();
	~MachineGunBullet();
	CREATE_FUNC(MachineGunBullet);
	virtual bool init();

protected:
	virtual void onLockTarget(Entity * target) override;

private:
	void moveEnd();
};

class FreezeArrowBullet : public Bullet
{
public:
	FreezeArrowBullet();
	~FreezeArrowBullet();
	CREATE_FUNC(FreezeArrowBullet);
	virtual bool init();
	
protected:
	virtual void onLockTarget(Entity * target) override;

private:
	void moveEnd();
};

class CannonRocketBullet : public Bullet
{
public:
	CannonRocketBullet();
	~CannonRocketBullet();
	CREATE_FUNC(CannonRocketBullet);
	virtual bool init();

protected:
	virtual void onLockTarget(Entity * target) override;

private:
	void moveEnd();
};