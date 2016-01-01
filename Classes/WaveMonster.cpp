// WaveMonster.cpp
#include "WaveMonster.h"

WaveMonster* WaveMonster::createWithTypeAndCount(MonsterType type, int count)
{
	auto waveMonster = new WaveMonster();
	if(waveMonster && waveMonster->initWithTypeAndCount(type,count))
	{
		waveMonster->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(waveMonster);
	}
	return waveMonster;
}

bool WaveMonster::init()
{
	if (!Node::init())
		return false;

	return true;
}

bool WaveMonster::initWithTypeAndCount(MonsterType type, int count)
{
	if (!WaveMonster::init())
		return false;

	m_type = type;
	m_count = count;

	return true;
}