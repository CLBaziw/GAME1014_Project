#pragma once
#ifndef __OBSTACLE__
#define __OBSTACLE__

#include "Sprite.h"
#include "ObstacleType.h"

class Obstacle final : public Sprite
{
public:
	Obstacle(ObstacleType obsType);
	~Obstacle();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void setPosition(int x, int y);

private:
	void m_buildAnimations();

	ObstacleType m_pObstacleType;
};

#endif // ! __OBSTACLE__



