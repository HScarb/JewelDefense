// Entity.cpp
#include "Entity.h"
USING_NS_CC;

Entity::Entity()
{
	m_sprite = nullptr;
	m_sName = "";
	m_iID = 0;
	m_iModelID = 0;
	m_isDead = false;
	m_iSpeed = 1;
	m_iLevel = 1;
	m_iHP = 1;
}

Entity::~Entity()
{
}

void Entity::bindSprite(Sprite* sprite)
{
	if (this->m_sprite != nullptr)
	{
		m_sprite->removeFromParentAndCleanup(true);
	}
	this->m_sprite = sprite;
	this->addChild(m_sprite);

	Size size = m_sprite->getContentSize();
	this->setContentSize(size);

	onBindSprite();
}

Sprite* Entity::getSprite()
{
	return this->m_sprite;
}

bool Entity::isDead()
{
	return this->m_isDead;
}

void Entity::onDead()
{
}

void Entity::onBindSprite()
{
}

void Entity::onHurt(int damage)
{
}