// SceneManager.h
#pragma once
#include "cocos2d.h"
#include "GlobalEnum.h"

class SceneManager:public cocos2d::Node
{
public:
	static SceneManager * getInstance();
	virtual bool init();
	void changeScene(SceneType enSceneType);

private:
	static SceneManager * m_sceneManager;
};
