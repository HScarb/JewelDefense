// TDHUDLayer.h
#pragma once
#include "cocos2d.h"
#include "GlobalEnum.h"
#include "cocostudio\CocoStudio.h"
#include "ui\CocosGUI.h"


class TDHUDLayer : public cocos2d::Layer
{
public:
	TDHUDLayer();
	~TDHUDLayer();
	virtual bool init();
	CREATE_FUNC(TDHUDLayer);

	static TDHUDLayer * getInstance();
	
	int getiMoneyNum();
	int getiLifeNum();

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

private:
	void recvRefreshMoneyNum(Ref* pData);
	void recvRefreshMonsterNum(Ref* pData);
	void recvRefreshLifeNum(Ref* pData);
	void recvAllMonsterDead(Ref* pData);

	TowerType m_tapedTower;
	cocos2d::ui::ImageView* m_machineGunTower;
	cocos2d::ui::ImageView* m_freezeTower;
	cocos2d::ui::ImageView* m_cannonTower;

	cocos2d::Sprite * m_pendTower;
	cocos2d::Sprite * m_pendRange;

	int m_iMoneyNum;
	int m_iMonsterNum;
	int m_iLifeNum;

	cocos2d::ui::Text* m_moneyLab;
	cocos2d::ui::Text* m_monsterLab;
	cocos2d::ui::Text* m_lifeLab;

private:
	static TDHUDLayer * m_TDHUDLayer;
};
