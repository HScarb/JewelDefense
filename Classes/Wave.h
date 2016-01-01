// Wave.h
#pragma once
#include "cocos2d.h"
#include "WaveMonster.h"

class Wave :public cocos2d::Node
{
public:
	~Wave();
	static Wave * createWithArray(cocos2d::Vector<WaveMonster*> list);

	cocos2d::Vector<WaveMonster*> m_waveList;
private:
	bool initWithList(cocos2d::Vector<WaveMonster*> list);
};