// GameScene.cpp
#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "TDMapLayer.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	// auto layer = GameScene::create();
	auto tdmapLayer = TDMapLayer::create();
	tdmapLayer->initData();
	scene->addChild(tdmapLayer);
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
		return false;

	auto UI = CSLoader::createNode("MainScene.csb");
	this->addChild(UI);

	return true;
}