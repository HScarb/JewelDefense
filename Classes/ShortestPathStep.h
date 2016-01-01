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
	bool isEqual(const ShortestPathStep * other) const;		// ���ҽ�������ShortestPathSteps�ķ���������ͬʱ��������ȣ��������Ǵ�������ͬ�ķ��飩��
	std::string getDescription() const;						// ������

	CC_SYNTHESIZE(cocos2d::Vec2, _position, Position);		// ���������
	CC_SYNTHESIZE(int, _gScore, GScore);					// ��ʼ�㵽��ǰ��ķ�������
	CC_SYNTHESIZE(int, _hScore, HScore);					// ��ǰ�㵽Ŀ���ķ����������
	CC_SYNTHESIZE(ShortestPathStep*, _parent, Parent);		// ������һ������
};
