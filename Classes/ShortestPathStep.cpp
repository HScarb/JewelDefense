// ShortestPathStep.cpp
#include "ShortestPathStep.h"
USING_NS_CC;

ShortestPathStep::ShortestPathStep() :
	_position(Vec2::ZERO),
	_gScore(0),
	_hScore(0),
	_parent(nullptr)
{
}

ShortestPathStep::~ShortestPathStep()
{
}

ShortestPathStep* ShortestPathStep::createWithPosition(const cocos2d::Vec2& pos)
{
	ShortestPathStep * pRet = new ShortestPathStep();
	if (pRet && pRet->initWithPosition(pos))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

bool ShortestPathStep::initWithPosition(const cocos2d::Vec2& pos)
{
	bool bRet = false;
	do
	{
		this->setPosition(pos);
		bRet = true;
	} while (0);

	return bRet;
}

int ShortestPathStep::getFScore() const
{
	return this->getGScore() + this->getHScore();
}

bool ShortestPathStep::isEqual(const ShortestPathStep* other) const
{
	return this->getPosition() == other->getPosition();
}

std::string ShortestPathStep::getDescription() const
{
	return StringUtils::format("pos=[%.0f;%.0f]  g=%d  h=%d  f=%d",
		this->getPosition().x, this->getPosition().y,
		this->getGScore(), this->getHScore(), this->getFScore());
}