// Monster.cpp
#include "Monster.h"
#include "CsvUtil.h"
#include "EnumTDProp.h"
#include "GlobalPath.h"
#include "GlobalConsts.h"
#include "GameMediator.h"
USING_NS_CC;

Monster::Monster()
{
	m_mapLayer = nullptr;
	m_iSpeed = 1;
	m_tileCoord = Vec2::ZERO;
}

Monster::~Monster()
{
}

bool Monster::init()
{
	return true;
}

Monster* Monster::createFromCsvFileByID(int iMonsterID)
{
	Monster * monster = new Monster();
	if(monster && monster->initFromCsvFileByID(iMonsterID))
	{
		monster->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(monster);
	}
	return monster;
}

bool Monster::initFromCsvFileByID(int iMonsterID)
{
	if (!Monster::init())
		return false;

	CsvUtil * csvUtil = CsvUtil::getInstance();
	
	std::string sMonsterID = StringUtils::format("%d", iMonsterID);
	// find the line where the ID is
	int iLine = csvUtil->findValueInWithLine(sMonsterID.c_str(),
		enMonsterPropConf_ID,PATH_CSV_MONSTER);

	if (iLine < 0)
		return false;

	setID(iMonsterID);
	setLevel(csvUtil->getInt(iLine, enMonsterPropConf_Level, PATH_CSV_MONSTER));
	setModelID(csvUtil->getInt(iLine, enMonsterPropConf_ModelID, PATH_CSV_MONSTER));
	setHP(csvUtil->getInt(iLine, enMonsterPropConf_Hp, PATH_CSV_MONSTER));
	setSpeed(csvUtil->getInt(iLine, enMonsterPropConf_Speed, PATH_CSV_MONSTER));
	
	m_startTileCoord = TILECOORD_START;
	m_endTileCoord = TILECOORD_END;

	hasRemoved = false;
	m_isMoveEnd = false;

	schedule(schedule_selector(Monster::monsterLogic, this), 0.1f);

	return true;
}

Rect Monster::getRect()
{
	Rect rect = Rect(this->getPosition().x - getSprite()->getContentSize().width * 0.5f,
		this->getPosition().y - getSprite()->getContentSize().height* 0.5f,
		getSprite()->getContentSize().width,
		getSprite()->getContentSize().height);
	return rect;
}

void Monster::hurtByDamage(int damage)
{
	if (m_isDead)
	{
		return;
	}
	if (this->hasRemoved)
	{
		return;
	}

	int iCurHP = getHP();
	int iAfterHP = iCurHP - damage;

	onHurt(damage);

	if (iAfterHP > 0)
	{
		setHP(iAfterHP);
	}
	else
	{
		m_isDead = true;
		onDead();
		removeSelf();
	}
}

void Monster::moveToTarget()
{
	auto startPos = m_mapLayer->positionForTileCoord(m_startTileCoord);
	auto endPos = m_mapLayer->positionForTileCoord(m_endTileCoord);
	float dis = ccpDistance(startPos, endPos);

	auto moveTo = MoveTo::create(dis / (float)m_iSpeed, endPos);
	auto reachEndCallback = CallFunc::create([&]()
	{
		removeSelf();
	});
	this->runAction(Sequence::create(moveTo,reachEndCallback,nullptr));
}

void Monster::monsterLogic(float dt)
{
	GameMediator * gm = GameMediator::getInstance();
	// set if monster is move end
	if(this->getPosition().x >= m_mapLayer->positionForTileCoord(TILECOORD_END).x)
	{
		m_isMoveEnd = true;
	}

	if(this->m_isMoveEnd)
	{
		unscheduleAllSelectors();

		NotificationCenter::getInstance()->postNotification("LifeChange", (Ref*)-1);
		removeSelf();
		return;
	}

	if(this->getHP() <= 0)
	{
		unscheduleAllSelectors();

		// change money and clear HPbar
		this->onDead();
		this->removeSelf();
	}
}

void Monster::removeSelf()
{
	if (hasRemoved)
		return;

	hasRemoved = true;
	GameMediator * gm = GameMediator::getInstance();
	gm->m_monsterList.eraseObject(this);

	this->stopAllActions();
	this->removeFromParentAndCleanup(true);
}

///////// AStar
void Monster::moveToward()
{
	this->stopAllActions();

	Vec2 fromTileCoord = m_mapLayer->tileCoordForPosition(this->getPosition());
	Vec2 toTileCoord = TILECOORD_END;

	if(fromTileCoord == toTileCoord)
	{
		return;
	}
	if(!m_mapLayer->isValidTileCoord(toTileCoord) || m_mapLayer->hasTowerAtTileCoord(toTileCoord))
	{
		return;
	}

	_spOpenSteps.clear();
	_spClosedSteps.clear();

	// 先添加当前方块坐标到open列表
	this->insertInOpenSteps(ShortestPathStep::createWithPosition(fromTileCoord));

	do
	{
		// 得到最小的F值的步骤
		// 因为是有序列表，第一步是最小的F值
		ShortestPathStep * currentStep = _spOpenSteps.at(0);

		// 添加当前步到closed列表
		_spClosedSteps.pushBack(currentStep);

		// 将它从open列表中移除
		_spOpenSteps.erase(0);

		// 如果当前步骤是目标坐标，那么完成
		if(currentStep->getPosition() == toTileCoord)
		{
			this->constructPathAndStartAnimationFromStep(currentStep);

			_spOpenSteps.clear();
			_spClosedSteps.clear();
			break;
		}

		// 得到当前步骤的相邻方块坐标
		PointArray * adjSteps = m_mapLayer->getTilesNextToTile(currentStep->getPosition());
		// 遍历相邻方块坐标
		for (ssize_t i = 0; i < adjSteps->count();++i)
		{
			ShortestPathStep * step = ShortestPathStep::createWithPosition(adjSteps->getControlPointAtIndex(i));

			// 检查步骤是否已经在closed列表
			if(this->getStepIndex(_spClosedSteps,step) != -1)
			{
				// 如果不在，检查下一个
				continue;
			}
			
			// 计算从当前步骤到此步骤的成本
			int moveCost = this->costToMoveFromStepToAdjacentStep(currentStep, step);

			// 检查此步骤是否已经在open列表
			ssize_t index = this->getStepIndex(_spOpenSteps, step);

			// 如果不在open列表，添加它
			if(index == -1)
			{
				// 设置当前步为上一步
				step->setParent(currentStep);

				// G = 上一步的G + 上一步到这里的成本(1)
				step->setGScore(currentStep->getGScore() + moveCost);

				// H = 从此步到目标方块坐标移动量的估算值
				step->setHScore(this->computeHScoreFromCoordToCoord(step->getPosition(), toTileCoord));

				// 添加到open列表(列表中任然从小到大)
				this->insertInOpenSteps(step);
			}
			else		// 如果在open列表
			{
				step = _spOpenSteps.at(index);

				// 检查G值是否低于当前步到此步的值
				if ((currentStep->getGScore() + moveCost) < step->getGScore())
				{
					// G值等于上一步G值 + 上一步到这里的成本
					step->setGScore(currentStep->getGScore() + moveCost);

					// 因为G值变了，F值也会跟着变
					// 为了保持open列表有序，需要将此步移除，再重新按序插入
					// 在移除之前，需要先保持引用
					step->retain();

					// 现在可以放心移除，不用担心被释放
					_spOpenSteps.erase(index);

					// 重新插入
					this->insertInOpenSteps(step);

					// 释放它，因为open列表应该持有它
					step->release();
				}
			}
		}
	} while (_spOpenSteps.size() > 0);

	return;

	if (_shortestPath.empty())
	{
		// hit wall
	}
}

void Monster::moveOneStep()
{
	// pop path
	moveToward();

	Vec2 currentPosition = m_mapLayer->tileCoordForPosition(this->getPosition());

	if (_shortestPath.size() == 0)
		return;

	ShortestPathStep * s = _shortestPath.at(0);

	Vec2 futurePosition = s->getPosition();
	Vec2 diff = futurePosition - currentPosition;
	if (abs(diff.x) > abs(diff.y))
	{
		if (diff.x > 0)
			this->setRotation(0);
		else
			this->setRotation(180);
	}
	else
	{
		if (diff.y > 0)
			this->setRotation(-90);
		else
			this->setRotation(90);
	}
	float moveDuration = ccpDistance(futurePosition, currentPosition) * TILE_WIDTH / (float)this->getSpeed();
	MoveTo * moveAction = MoveTo::create(moveDuration, m_mapLayer->positionForTileCoord(s->getPosition()));
	CallFunc * moveCallback = CallFunc::create(CC_CALLBACK_0(Monster::moveOneStep, this));

	_shortestPath.erase(0);

	this->runAction(Sequence::create(moveAction, moveCallback, nullptr));
}

void Monster::insertInOpenSteps(ShortestPathStep* step)
{
	int stepFScore = step->getFScore();
	ssize_t count = _spOpenSteps.size();

	ssize_t i = 0;
	for (; i < count;++i)
	{
		if(stepFScore <= _spOpenSteps.at(i)->getFScore())
		{
			break;
		}
	}
	_spOpenSteps.insert(i, step);
}

int Monster::computeHScoreFromCoordToCoord(const cocos2d::Vec2& fromCoord, const cocos2d::Vec2 toCoord)
{
	return abs(toCoord.x - fromCoord.x) + abs(toCoord.y - fromCoord.y);
}

int Monster::costToMoveFromStepToAdjacentStep(const ShortestPathStep* fromStep, const ShortestPathStep* toStep)
{
	return 1;
}

ssize_t Monster::getStepIndex(const cocos2d::Vector<ShortestPathStep*>& steps, const ShortestPathStep* step)
{
	for (ssize_t i = 0; i < steps.size();++i)
	{
		if (steps.at(i)->isEqual(step))
			return i;
	}
	return -1;
}

void Monster::constructPathAndStartAnimationFromStep(ShortestPathStep* step)
{
	_shortestPath.clear();

	do
	{
		if (step->getParent())
		{
			_shortestPath.insert(0, step);
		}
		step = step->getParent();
	} while (step);

//	for (const ShortestPathStep *s : _shortestPath)
//	{
//		CCLOG("%s", s->getDescription().c_str());
//	}
}

void Monster::popStepAnimate()
{
}

//////// HP bar
void Monster::onDead()
{

	int iMoneyChange = 3 * this->getLevel();
	NotificationCenter::getInstance()->postNotification("MoneyChange", (Ref*)iMoneyChange);
}

void Monster::onBindSprite()
{
	m_iMaxHP = getHP();
}

void Monster::onHurt(int damage)
{
}