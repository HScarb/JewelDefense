// LoadingScene.cpp
#include "LoadingScene.h"
#include "GlobalConsts.h"
#include "GlobalPath.h"
#include "SimpleAudioEngine.h"
#include "SceneManager.h"
USING_NS_CC;
using namespace CocosDenshion;

Scene* LoadingScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoadingScene::create();
	scene->addChild(layer);
	return scene;
}

bool LoadingScene::init()
{
	if (!Layer::init())
		return false;

	auto winSize = Director::getInstance()->getWinSize();
	auto label = Label::createWithSystemFont("Loading...", "Courier", 36);
	label->setPosition(winSize.width / 2, winSize.height / 2);
	addChild(label);

	int *ptexture_num = &m_texture_num;

	auto addTextureCallback = [ptexture_num](Texture2D * texture)
	{
		(*ptexture_num)++;
		log("loaded a texture async");
	};

	// start checker
	schedule(schedule_selector(LoadingScene::onTextureLoading));

	// async load textures
	// ----------- TD -----------
	TextureCache::getInstance()->addImageAsync(PATH_MONSTER_RED, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_MONSTER_GREEN, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_MONSTER_BROWN, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_TOWER_MATHINEGUN, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_TOWER_FREEZE, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_TOWER_CANNON, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_BULLET_MATHINEGUN, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_BULLET_FREEZEARROW, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_BULLET_ROCKET, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_RANGE, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_STAR, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_FIRE, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_MAP_GRID, addTextureCallback);
	// ------------ Jewel ------------
	TextureCache::getInstance()->addImageAsync(PATH_JEWEL_BG_HEAD, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_JEWEL_1, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_JEWEL_2, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_JEWEL_3, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_JEWEL_4, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_JEWEL_5, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_JEWEL_6, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_SELECTED_RED, addTextureCallback);
	TextureCache::getInstance()->addImageAsync(PATH_SELECTED_WHITE, addTextureCallback);


	// sounds
	SimpleAudioEngine::getInstance()->preloadEffect(PATH_SOUND_IMPACT);
	SimpleAudioEngine::getInstance()->preloadEffect(PATH_SOUND_ELIMATE);
	SimpleAudioEngine::getInstance()->preloadEffect(PATH_SOUND_ERROR);
	SimpleAudioEngine::getInstance()->preloadEffect(PATH_SOUND_SWAPBACK);

	return true;
}

void LoadingScene::onTextureLoading(float dt)
{
	if(m_texture_num >= TOTAL_TEXTURE_NUM)
	{
		unschedule(schedule_selector(LoadingScene::onTextureLoading));
		CCLOG("Load finish!");

		auto call = CallFunc::create([]()
		{
			SceneManager::getInstance()->changeScene(en_GameScene);
		});
		// enter scene
		this->runAction(Sequence::create(DelayTime::create(0.5), call, nullptr));
	}
}