// GameMediator.h
// GameMediator.h
#pragma once
#include "cocos2d.h"
#include "TDMapLayer.h"
#include "Monster.h"
#include "EnumTDProp.h"
#include "MonsterData.h"
class Tower;

class GameMediator : public cocos2d::Ref
{
public:
	~GameMediator();
	static GameMediator * getInstance();
	bool init();

	CC_SYNTHESIZE_RETAIN(TDMapLayer*, m_mapLayer, MapLayer);
	
	cocos2d::Vector<Tower*> m_towerList;
	cocos2d::Vector<Monster*> m_monsterList;
	cocos2d::Vector<MonsterData*> m_monsterDataList;

private:
	static GameMediator * m_gameMediator;
};
