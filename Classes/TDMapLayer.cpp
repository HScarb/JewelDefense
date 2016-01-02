// TDMapLayer.cpp
#include "TDMapLayer.h"
#include "GameMediator.h"
#include "GlobalConsts.h"
#include "Tower.h"
#include "TowerNew.h"
#include "TowerManager.h"
USING_NS_CC;

TDMapLayer::TDMapLayer()
{
	m_iCurLevel = 1;
	m_HUDLayer = nullptr;
	m_map = nullptr;
	m_metaLayer = nullptr;
}

TDMapLayer::~TDMapLayer()
{
}

bool TDMapLayer::init()
{
	if (!Layer::init())
		return false;

	/*schdule update*/
	
	// set game mediator
	GameMediator::getInstance()->setMapLayer(this);

	// load map
	m_map = TMXTiledMap::create("TD_TiledMap.tmx");
	m_map->setAnchorPoint(Vec2(0, 0));
	int mapWidth = m_map->getMapSize().width * m_map->getTileSize().width;
	int mapHeight = m_map->getMapSize().height * m_map->getTileSize().height;
	m_map->setPosition(0, 0);

	m_metaLayer = m_map->getLayer("metaLayer");
	m_metaLayer->setVisible(false);
	this->addChild(m_map, 2);

	// load game HUD layer
	m_HUDLayer = TDHUDLayer::create();
	this->addChild(m_HUDLayer, 5);
	
	// create tower manager
	m_towerManager = TowerManager::create();
	this->addChild(m_towerManager, LEVEL_TOWER_LAYER);

	// load monsters
	loadMonsterDataByLevel(1);

	schedule(schedule_selector(TDMapLayer::logic));

	return true;
}

void TDMapLayer::initData()
{
	int iMoneyNum = 5;
	int iMonsterNum = 0;
	int iLifeNum = 100;

	NotificationCenter::getInstance()->postNotification("MoneyChange", (Ref*)iMoneyNum);
	NotificationCenter::getInstance()->postNotification("MonsterNumChange", (Ref*)iMonsterNum);
	NotificationCenter::getInstance()->postNotification("LifeChange", (Ref*)iLifeNum);
}

bool TDMapLayer::canBuildOnPosition(cocos2d::Vec2 position)
{
	return true;
}

bool TDMapLayer::buildTowerOnPos(cocos2d::Vec2 position, TowerType towerType)
{
	Vec2 tileCoord = tileCoordForPosition(position);
	if(!isValidTileCoord(tileCoord)
		||!isGroundAtTileCoord(tileCoord)
		||hasMonsterAtTileCoord(tileCoord)
		||hasTowerAtTileCoord(tileCoord))
	{
		return false;
	}

	// build tower
	Tower * tempTower = nullptr;

	switch(towerType)
	{
	case en_NoneTower: break;
	case en_MachineGunTower: 
		tempTower = MachineGunTower::create();
		break;
	case en_FreezeTower: 
		tempTower = FreezeTower::create();
		break;
	case en_CannonTower: 
		tempTower = CannonTower::create();
		break;
	default: break;
	}
	Vec2 accurPos = positionForTileCoord(tileCoord);
	tempTower->setTileCoord(tileCoord);
	tempTower->setPosition(accurPos);
	
	m_towerManager->addToTowerList(tempTower);
	this->addChild(tempTower, LEVEL_TOWER_LAYER);

	return true;
}

Vec2 TDMapLayer::tileCoordForPosition(cocos2d::Vec2 position)
{
	int x = position.x / m_map->getTileSize().width;
	int y = position.y / m_map->getTileSize().height;
	y = 7 - y;
	return Vec2(x, y);
}

Vec2 TDMapLayer::positionForTileCoord(cocos2d::Vec2 tileCoord)
{
	float x = tileCoord.x * m_map->getTileSize().width + m_map->getTileSize().width / 2;
	float y = (7 - tileCoord.y) * m_map->getTileSize().height + m_map->getTileSize().height / 2;
	return Vec2(x, y);
}

PointArray * TDMapLayer::getTilesNextToTile(cocos2d::Vec2 tileCoord)
{
	PointArray * temp = PointArray::create(4);

	// up
	Vec2 p(tileCoord.x, tileCoord.y - 1);
	if(this->isValidTileCoord(p) && !this->isWallAtTileCoord(p) && !this->hasTowerAtTileCoord(p))
	{
		temp->addControlPoint(p);
	}

	// left
	p.setPoint(tileCoord.x - 1, tileCoord.y);
	if (this->isValidTileCoord(p) && !this->isWallAtTileCoord(p) && !this->hasTowerAtTileCoord(p))
	{
		temp->addControlPoint(p);
	}

	// down
	p.setPoint(tileCoord.x, tileCoord.y + 1);
	if (this->isValidTileCoord(p) && !this->isWallAtTileCoord(p) && !this->hasTowerAtTileCoord(p))
	{
		temp->addControlPoint(p);
	}

	// right
	p.setPoint(tileCoord.x + 1, tileCoord.y);
	if (this->isValidTileCoord(p) && !this->isWallAtTileCoord(p) && !this->hasTowerAtTileCoord(p))
	{
		temp->addControlPoint(p);
	}

	return temp;
}

bool TDMapLayer::isOutOfMap(cocos2d::Vec2 pos)
{
	return false;
}

bool TDMapLayer::isValidTileCoord(const cocos2d::Vec2 tileCoord) const
{
	if (tileCoord.x < 0 || tileCoord.x > 11)
		return false;
	else if (tileCoord.y < 0 || tileCoord.y > 7)
		return false;
	return true;
}

bool TDMapLayer::isWallAtTileCoord(const cocos2d::Vec2 tileCoord) const
{
	return this->isPropAtTileCoordForLayer("Wall", tileCoord, m_metaLayer);
}

bool TDMapLayer::isGroundAtTileCoord(const cocos2d::Vec2 tileCoord) const
{
	return this->isPropAtTileCoordForLayer("Ground", tileCoord, m_metaLayer);
}

bool TDMapLayer::hasTowerAtTileCoord(const cocos2d::Vec2 tileCoord) const
{
	return false;
}

bool TDMapLayer::hasMonsterAtTileCoord(const cocos2d::Vec2 tileCoord) const
{
	return false;
}

bool TDMapLayer::isPropAtTileCoordForLayer(const char* prop, const cocos2d::Vec2 tileCoord, cocos2d::TMXLayer* layer) const
{
	if (!this->isValidTileCoord(tileCoord))
		return false;

	int gid = m_metaLayer->getTileGIDAt(tileCoord);
	Value properties = m_map->getPropertiesForGID(gid);
	if (properties.isNull())
		return false;
	return properties.asValueMap().find(prop) != properties.asValueMap().end();
}

void TDMapLayer::addMonster(MonsterData * monsterData)
{
	auto gm = GameMediator::getInstance();
	auto monster = Monster::createFromCsvFileByID(monsterData->iID);
	
	gm->m_monsterList.pushBack(monster);
	gm->m_monsterDataList.eraseObject(monsterData);

	// bind sprite
	std::string sPath = StringUtils::format("sprite/monster/monster_%d.png", monster->getModelID());
	monster->bindSprite(Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey(sPath.c_str())));

	// setMapLayer
	monster->setMapLayer(this);
	monster->setPosition(positionForTileCoord(TILECOORD_START));
	this->addChild(monster, LEVEL_MONSTER_LAYER);
	
//	monster->moveToTarget();
	monster->moveOneStep();
}

void TDMapLayer::loadMonsterDataByLevel(int level)
{
	auto gm = GameMediator::getInstance();
	gm->m_monsterDataList.clear();

	std::string sMonsterConfPath = StringUtils::format("tollgate/monster_level_%d.plist", level);

	ValueMap fileDataMap = FileUtils::getInstance()->getValueMapFromFile(sMonsterConfPath.c_str());

	int size = fileDataMap.size();
	for (int i = 1; i <= size;i++)
	{
		Value value = fileDataMap.at(StringUtils::toString(i));
		ValueMap data = value.asValueMap();

		MonsterData * monsterData = new MonsterData();
		// read monsterID & monster show time from config and save all to monster data list
		monsterData->iID = data["id"].asInt();
		monsterData->fShowTime = data["showTime"].asFloat();
		gm->m_monsterDataList.pushBack(monsterData);
		
		monsterData = nullptr;
	}
}

void TDMapLayer::logic(float dt)
{
	auto gm = GameMediator::getInstance();
	static float totalTime = 0.0;
	totalTime += dt;

//	long now = millisecondNow();
//	static long lastTimeMonsterAdded = now;
//	CCLOG("%ld", now - lastTimeMonsterAdded);
	for(auto monsterD:gm->m_monsterDataList)
	{
		if(totalTime >= monsterD->fShowTime)
		{
			addMonster(monsterD);
			CCLOG("Add monster at time %f", totalTime);
			NotificationCenter::getInstance()->postNotification("MonsterNumChange",(Ref*)1);
		}
	}
	if (gm->m_monsterDataList.size() + gm->m_monsterList.size() == 0)
	{
		NotificationCenter::getInstance()->postNotification("AllMonsterDead");
	}
	// tower logic
//	m_towerManager->towerLogic(dt);

}