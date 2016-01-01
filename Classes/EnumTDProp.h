// EnumTDProp.h
#pragma once

enum EnumTowerPropConfType {
	enTowerPropConf_ID,				// 英雄ID
	enTowerPropConf_Name,			// 英雄名字
	enTowerPropConf_Type,			// 英雄类型
	enTowerPropConf_ModelID,			// 英雄模型ID
	enTowerPropConf_BaseAtk,			// 基础攻击力
	enTowerPropConf_AtkSpeed,		// 攻击间隔（单位：秒）
	enTowerPropConf_AtkRange,		// 攻击范围（半径）
	enTowerPropConf_UpgradeAtkBase,	// 升级攻击加成系数
	enTowerPropConf_UpgradeCostBase,	// 升级消耗基础值
};

enum EnumMonsterPropConfType
{
	enMonsterPropConf_ID,		// 怪物ID
	enMonsterPropConf_Name,		// 怪物名字
	enMonsterPropConf_Level,	// 怪物等级
	enMonsterPropConf_Type,		// 怪物类型
	enMonsterPropConf_ModelID,	// 怪物模型ID
	enMonsterPropConf_Defense,	// 防御力
	enMonsterPropConf_Hp,	    // 血量
	enMonsterPropConf_Speed,	// 移动速度
};