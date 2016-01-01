// ShortestPathStep.h
#pragma once
#include "cocos2d.h"

class ShortestPathStep : public cocos2d::Ref
{
public:
	ShortestPathStep();
	~ShortestPathStep();
	static ShortestPathStep * createWithPosition(const cocos2d::Vec2 &pos);
	bool initWithPosition(const cocos2d::Vec2 & pos);

	int getFScore() const;									// return gScore + hScore
	bool isEqual(const ShortestPathStep * other) const;		// 当且仅当两个ShortestPathSteps的方块坐标相同时，它们相等（例如它们代表着相同的方块）。
	std::string getDescription() const;						// 调试用

	CC_SYNTHESIZE(cocos2d::Vec2, _position, Position);		// 方块的坐标
	CC_SYNTHESIZE(int, _gScore, GScore);					// 开始点到当前点的方块数量
	CC_SYNTHESIZE(int, _hScore, HScore);					// 当前点到目标点的方块估算数量
	CC_SYNTHESIZE(ShortestPathStep*, _parent, Parent);		// 它的上一步操作
};
