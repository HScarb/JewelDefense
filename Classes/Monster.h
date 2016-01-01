// Monster.h
#pragma once
#include "Monster.h"
#include "Entity.h"
#include "TDMapLayer.h"
#include "GlobalConsts.h"
#include "ShortestPathStep.h"

class Monster : public Entity
{
public:
	Monster();
	~Monster();
	CREATE_FUNC(Monster);
	virtual bool init();

	static Monster* createFromCsvFileByID(int iMonsterID);
	bool initFromCsvFileByID(int iMonsterID);

public:
	cocos2d::Rect getRect();
	void hurtByDamage(int damage);
	void moveToTarget();

	void monsterLogic(float dt);
	
	void setMapLayer(TDMapLayer * mapLayer) { m_mapLayer = mapLayer; }

protected:
	void removeSelf();

	bool hasRemoved;
public:
	// MapLayer
	TDMapLayer * m_mapLayer;
	
	CC_SYNTHESIZE(cocos2d::Vec2, m_startTileCoord, StartTileCoord);
	CC_SYNTHESIZE(cocos2d::Vec2, m_endTileCoord, EndTileCoord);
	CC_SYNTHESIZE(cocos2d::Vec2, m_tileCoord, TileCoord);
	CC_SYNTHESIZE_BOOL(m_isMoveEnd, MoveEnd);

// ---------- AStar -----------
public:
	void moveToward();
	void moveOneStep();

protected:
	void insertInOpenSteps(ShortestPathStep * step);
	int computeHScoreFromCoordToCoord(const cocos2d::Vec2 & fromCoord, const cocos2d::Vec2 toCoord);
	int costToMoveFromStepToAdjacentStep(const ShortestPathStep * fromStep, const ShortestPathStep * toStep);

	ssize_t getStepIndex(const cocos2d::Vector<ShortestPathStep*> &steps, const ShortestPathStep * step);
	void constructPathAndStartAnimationFromStep(ShortestPathStep * step);
	void popStepAnimate();

	cocos2d::Vector<ShortestPathStep*> _spOpenSteps;
	cocos2d::Vector<ShortestPathStep*> _spClosedSteps;
	cocos2d::Vector<ShortestPathStep*> _shortestPath;

// ---------- Hp Bar ----------
protected:
	virtual void onDead() override;
	virtual void onBindSprite() override;
	virtual void onHurt(int damage) override;

	cocos2d::ui::LoadingBar * m_HPBar;
	int m_iMaxHP;
};
