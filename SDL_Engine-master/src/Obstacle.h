#pragma once
#ifndef __OBSTACLE__
#define __OBSTACLE__

#include "Sprite.h"
#include "GameObjectType.h"
#include "EnemyAnimationState.h"

class Obstacle final : public Sprite
{
public:
	Obstacle(GameObjectType obsType);
	~Obstacle();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void setPosition(int x, int y);

private:
	void m_buildAnimations();

	GameObjectType m_pObstacleType;
	EnemyAnimationState m_currentAnimationState;
};

#endif // ! __OBSTACLE__



