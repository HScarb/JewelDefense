// GlobalConsts.h
#pragma once

#define TOTAL_TEXTURE_NUM 22

#define TILE_RADIUS 22.5
#define TILE_WIDTH 45

// levels
#define LEVEL_TDMAP 5
#define LEVEL_TDHUDLAYER 6
#define LEVEL_HOME_LAYER 8		// ���ݵĲ��
#define LEVEL_TOWER_LAYER 10		// Ӣ�۵Ĳ��
#define LEVEL_MONSTER_LAYER 15    // ����Ĳ��
#define LEVEL_BULLET_LAYER 20		// �ӵ��Ĳ��
#define LEVEL_PENDRANGE 30		// �����еķ�Χ
#define LEVEL_PENDTOWER 31		// �����е���

// bullet speed
#define BULLETSPEED_MACHINEGUN 400
#define BULLETSPEED_FREEZEARROW 350
#define BULLETSPEED_CANNONROCKET 300

// start & end tile coordinate
#define TILECOORD_START Vec2(0,3)
#define TILECOORD_END Vec2(11,3)

// create bool variable, include get & set method
#define CC_SYNTHESIZE_BOOL(varName, funName)\
protected: bool varName; \
public: bool is##funName(void) const { return varName; } \
public: void set##funName(bool var){ varName = var; }