// BulletManager.h
#pragma once
#include "cocos2d.h"
#include "Bullet.h"
#define BULLET_MAX_CACHE_NUM 10

class BulletManager : public cocos2d::Node
{
public:
	BulletManager();
	~BulletManager();

	static BulletManager * create();
	bool init();

	Bullet * getAnyUnUsedBullet();
	Bullet * getBulletByType(BulletType bulletType);

private:
	cocos2d::Vector<Bullet*> m_bulletList;
	void createBullets();
	void bulletLogic(float dt);
};