// TDMapLayer.h
#pragma once
#include "cocos2d.h"
#include "TDHUDLayer.h"
class TowerManager;
class MonsterData;
class GameMediator;

class TDMapLayer : public cocos2d::Layer
{
public:
	TDMapLayer();
	~TDMapLayer();
	CREATE_FUNC(TDMapLayer);
	virtual bool init();

	void initData();

	bool canBuildOnPosition(cocos2d::Vec2 position);
	bool buildTowerOnPos(cocos2d::Vec2 position, TowerType towerType);


	cocos2d::Vec2 tileCoordForPosition(cocos2d::Vec2 position);
	cocos2d::Vec2 positionForTileCoord(cocos2d::Vec2 tileCoord);
	cocos2d::PointArray * getTilesNextToTile(cocos2d::Vec2 tileCoord);

	bool isOutOfMap(cocos2d::Vec2 pos);

public:
//	long millisecondNow()
//	{
//		timeval now;
//		cocos2d::gettimeofday(&now, nullptr);
//		return (now.tv_sec * 1000 + now.tv_usec / 1000);
//	}

	bool isValidTileCoord(const cocos2d::Vec2 tileCoord) const;
	bool isWallAtTileCoord(const cocos2d::Vec2 tileCoord) const;
	bool isGroundAtTileCoord(const cocos2d::Vec2 tileCoord) const;
	bool hasTowerAtTileCoord(const cocos2d::Vec2 tileCoord) const;
	bool hasMonsterAtTileCoord(const cocos2d::Vec2 tileCoord) const;

private:
	bool isPropAtTileCoordForLayer(const char * prop, const cocos2d::Vec2 tileCoord, cocos2d::TMXLayer * layer) const;

	void addMonster(MonsterData * monsterData);
	void loadMonsterDataByLevel(int level);

	void logic(float dt);

private:
	int m_iCurLevel;
	TowerManager * m_towerManager;
	TDHUDLayer * m_HUDLayer;

	// tiled map
	cocos2d::TMXTiledMap * m_map;
	cocos2d::TMXLayer * m_metaLayer;
};
