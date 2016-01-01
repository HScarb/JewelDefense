// MonsterData.h
#pragma once
#include "cocos2d.h"

// including iID and fShowTime
class MonsterData : public cocos2d::Ref
{
public:
	MonsterData() { iID = -1; fShowTime = -1; }
	~MonsterData(){}
	int iID;
	float fShowTime;
};
