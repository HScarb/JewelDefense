// Entity.h
#pragma once
#include "cocos2d.h"

class Entity : public cocos2d::Node
{
public:
	Entity();
	~Entity();

	void bindSprite(cocos2d::Sprite * sprite);
	cocos2d::Sprite* getSprite();

	bool isDead();
protected:
	virtual void onDead();
	virtual void onBindSprite();
	virtual void onHurt(int damage);

private:
	cocos2d::Sprite * m_sprite;

	CC_SYNTHESIZE(int, m_iID, ID);
	CC_SYNTHESIZE(int, m_iModelID, ModelID);
	CC_SYNTHESIZE(std::string, m_sName, Name);
	CC_SYNTHESIZE(int, m_iHP, HP);
	CC_SYNTHESIZE(int, m_iSpeed, Speed);
	CC_SYNTHESIZE(int, m_iLevel, Level);
	bool m_isDead;
};
