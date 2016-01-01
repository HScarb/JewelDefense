// WaveMonster.h
#pragma once
#include "cocos2d.h"
#include "GlobalEnum.h"

class WaveMonster : public cocos2d::Node
{
public:
	static WaveMonster * createWithTypeAndCount(MonsterType type, int count);
	virtual bool init();
	CREATE_FUNC(WaveMonster);
	
	MonsterType m_type;
	int m_count;

private:
	bool initWithTypeAndCount(MonsterType type, int count);
};