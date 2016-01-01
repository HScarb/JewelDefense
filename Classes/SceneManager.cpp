// SceneManager.cpp
#include "SceneManager.h"
#include "LoadingScene.h"
#include "GameScene.h"
USING_NS_CC;

SceneManager * SceneManager::m_sceneManager = nullptr;

SceneManager* SceneManager::getInstance()
{
	if(m_sceneManager == nullptr)
	{
		m_sceneManager = new SceneManager;
		if(m_sceneManager && m_sceneManager->init())
		{
			m_sceneManager->autorelease();
			m_sceneManager->retain();
		}
		else
		{
			CC_SAFE_DELETE(m_sceneManager);
			m_sceneManager = nullptr;
		}
	}
	return m_sceneManager;
}

bool SceneManager::init()
{
	return true;
}

void SceneManager::changeScene(SceneType enSceneType)
{
	Scene * pScene = nullptr;

	switch (enSceneType)
	{
	case en_NoneScene:
		break;
	case en_LoadingScene: 
		pScene = LoadingScene::createScene();
		break;
	case en_SelectScene: 
		break;
	case en_GameScene: 
		pScene = GameScene::createScene();
		break;
	case en_WinScene: break;
	case en_LoseScene: break;
	default: break;
	}
	
	if (pScene == nullptr)
		return;

	Director * pDirector = Director::getInstance();
	Scene * curScene = pDirector->getRunningScene();

	if(curScene == nullptr)
	{
		pDirector->runWithScene(pScene);
	}
	else
	{
		pDirector->replaceScene(pScene);
	}
}