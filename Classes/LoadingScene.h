// LoadingScene.h
#pragma once
#include "cocos2d.h"

class LoadingScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(LoadingScene);

private:
	int m_texture_num;

	void onTextureLoading(float dt);
};