// Projectile.h
#pragma once
#include "cocos2d.h"
class Monster;
class GameMediator;

class Projectile : public cocos2d::Node
{
public:
	virtual bool initWithFile(const char * filename);
	
	virtual cocos2d::Rect getRect();

	CC_SYNTHESIZE(int, m_iSpeed, Speed);
	CC_SYNTHESIZE(cocos2d::Vec2, m_targetPos, TargetPos);
	CC_SYNTHESIZE(int, m_iDamage, Damage);

protected:
	cocos2d::Sprite * m_sprite;
	GameMediator * gm;
	
	bool hasRemoved;

	void removeSelf();
};

class MachineGunProjectile: public Projectile
{
public:
	static MachineGunProjectile * createWithTargetPos(cocos2d::Vec2 pos);
	bool initWithTargetPos(cocos2d::Vec2 pos);
	void update(float delta);

private:
	void moveToTargetPos();
};

class FreezeProjectile:public Projectile
{
public:
	static FreezeProjectile * createWithTargetPos(cocos2d::Vec2 pos);
	bool initWithTargetPos(cocos2d::Vec2 pos);
	void update(float delta);

private:
	void moveToTargetPos();
};

class CannonProjectile:public Projectile
{
public:
	static CannonProjectile * createWithTarget(Monster * monster);
	bool initWithTarget(Monster * monster);
	void update(float delta);

private:
	float angularVelocity;

	float to360Angle(float angle)
	{
		angle = (int)angle % 360;
		if (angle < 0)
			angle += 360;
		return angle;
	}
	Monster * m_target;
};