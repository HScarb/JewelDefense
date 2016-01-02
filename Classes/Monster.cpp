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

	// ����ӵ�ǰ�������굽open�б�
	this->insertInOpenSteps(ShortestPathStep::createWithPosition(fromTileCoord));

	do
	{
		// �õ���С��Fֵ�Ĳ���
		// ��Ϊ�������б���һ������С��Fֵ
		ShortestPathStep * currentStep = _spOpenSteps.at(0);

		// ��ӵ�ǰ����closed�б�
		_spClosedSteps.pushBack(currentStep);

		// ������open�б����Ƴ�
		_spOpenSteps.erase(0);

		// �����ǰ������Ŀ�����꣬��ô���
		if(currentStep->getPosition() == toTileCoord)
		{
			this->constructPathAndStartAnimationFromStep(currentStep);

			_spOpenSteps.clear();
			_spClosedSteps.clear();
			break;
		}

		// �õ���ǰ��������ڷ�������
		PointArray * adjSteps = m_mapLayer->getTilesNextToTile(currentStep->getPosition());
		// �������ڷ�������
		for (ssize_t i = 0; i < adjSteps->count();++i)
		{
			ShortestPathStep * step = ShortestPathStep::createWithPosition(adjSteps->getControlPointAtIndex(i));

			// ��鲽���Ƿ��Ѿ���closed�б�
			if(this->getStepIndex(_spClosedSteps,step) != -1)
			{
				// ������ڣ������һ��
				continue;
			}
			
			// ����ӵ�ǰ���赽�˲���ĳɱ�
			int moveCost = this->costToMoveFromStepToAdjacentStep(currentStep, step);

			// ���˲����Ƿ��Ѿ���open�б�
			ssize_t index = this->getStepIndex(_spOpenSteps, step);

			// �������open�б������
			if(index == -1)
			{
				// ���õ�ǰ��Ϊ��һ��
				step->setParent(currentStep);

				// G = ��һ����G + ��һ��������ĳɱ�(1)
				step->setGScore(currentStep->getGScore() + moveCost);

				// H = �Ӵ˲���Ŀ�귽�������ƶ����Ĺ���ֵ
				step->setHScore(this->computeHScoreFromCoordToCoord(step->getPosition(), toTileCoord));

				// ��ӵ�open�б�(�б�����Ȼ��С����)
				this->insertInOpenSteps(step);
			}
			else		// �����open�б�
			{
				step = _spOpenSteps.at(index);

				// ���Gֵ�Ƿ���ڵ�ǰ�����˲���ֵ
				if ((currentStep->getGScore() + moveCost) < step->getGScore())
				{
					// Gֵ������һ��Gֵ + ��һ��������ĳɱ�
					step->setGScore(currentStep->getGScore() + moveCost);

					// ��ΪGֵ���ˣ�FֵҲ����ű�
					// Ϊ�˱���open�б�������Ҫ���˲��Ƴ��������°������
					// ���Ƴ�֮ǰ����Ҫ�ȱ�������
					step->retain();

					// ���ڿ��Է����Ƴ������õ��ı��ͷ�
					_spOpenSteps.erase(index);

					// ���²���
					this->insertInOpenSteps(step);

					// �ͷ�������Ϊopen�б�Ӧ�ó�����
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