// Wave.cpp
#include "Wave.h"

Wave::~Wave()
{
}

Wave* Wave::createWithArray(cocos2d::Vector<WaveMonster*> list)
{
	Wave * wave = new Wave();
	if(wave && wave->init())
	{
		wave->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(wave);
	}
	return wave;
}

bool Wave::initWithList(cocos2d::Vector<WaveMonster*> list)
{
	m_waveList = list;
	return true;
}