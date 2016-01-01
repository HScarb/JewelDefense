// GameMediator.cpp
#include "GameMediator.h"
#include "TowerNew.h"

GameMediator * GameMediator::m_gameMediator = nullptr;

GameMediator::~GameMediator()
{
	CC_SAFE_RELEASE_NULL(m_mapLayer);

}

GameMediator* GameMediator::getInstance()
{
	if (m_gameMediator == nullptr)
	{
		m_gameMediator = new GameMediator;
		if (m_gameMediator && m_gameMediator->init())
		{
			m_gameMediator->autorelease();
			m_gameMediator->retain();
		}
		else
		{
			CC_SAFE_DELETE(m_gameMediator);
			m_gameMediator = nullptr;
		}
	}
	return m_gameMediator;
}

bool GameMediator::init()
{
	bool bRet = false;
	do
	{
		m_mapLayer = nullptr;

		bRet = true;
	} while (0);
	return bRet;
}
