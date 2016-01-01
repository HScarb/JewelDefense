// BulletManager.cpp
#include "BulletManager.h"
#include "Monster.h"
#include "GameMediator.h"
USING_NS_CC;

BulletManager::BulletManager()
{
}

BulletManager::~BulletManager()
{
}

BulletManager* BulletManager::create()
{
	BulletManager* bulletMgr = new BulletManager();
	if (bulletMgr && bulletMgr->init())
	{
		bulletMgr->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(bulletMgr);
	}
	return bulletMgr;
}

bool BulletManager::init()
{
	if (!Node::init())
		return false;

	createBullets();

	// 循环检测子弹逻辑
	this->schedule(schedule_selector(BulletManager::bulletLogic));

	return true;
}

Bullet* BulletManager::getAnyUnUsedBullet()
{
	for (auto bullet : m_bulletList)
	{
		if (bullet->getIsUsing() == false)
		{
			bullet->setIsUsing(true);
			return bullet;
		}
	}
	return nullptr;
}

Bullet* BulletManager::getBulletByType(BulletType bulletType)
{
	for (auto bullet : m_bulletList)
	{
		if (bullet->getIsUsing() == false)
		{
			if (bullet->getBulletType() == bulletType)
			{
				bullet->setIsUsing(true);
				return bullet;
			}
		}
	}
	return nullptr;
}

void BulletManager::createBullets()
{
	Bullet* bullet = nullptr;
	for (int i = 0; i < BULLET_MAX_CACHE_NUM; i++)
	{
		bullet = MachineGunBullet::create();
		bullet->setIsUsing(false);
		bullet->setVisible(false);
		m_bulletList.pushBack(bullet);
		this->addChild(bullet);

		bullet = FreezeArrowBullet::create();
		bullet->setIsUsing(false);
		bullet->setVisible(false);
		m_bulletList.pushBack(bullet);
		this->addChild(bullet);

		bullet = CannonRocketBullet::create();
		bullet->setIsUsing(false);
		bullet->setVisible(false);
		m_bulletList.pushBack(bullet);
		this->addChild(bullet);
	}
}

void BulletManager::bulletLogic(float dt)
{
	auto monsterList = GameMediator::getInstance()->m_monsterList;
	auto monsterDataList = GameMediator::getInstance()->m_monsterDataList;
	for (auto bullet : m_bulletList)
	{
		if (bullet->getIsUsing())
		{
			auto target = bullet->getTarget();

			if (target != nullptr && !target->isDead())
			{
				// 判断子弹是否达到了目标处，是的话，伤害目标(子弹使用MoveTo移动)
				if (bullet->isReached())
				{
					target->hurtByDamage(bullet->getDamage());

					// reset to unUsed
					bullet->setIsUsing(false);
				}
			}
		}
	}
}