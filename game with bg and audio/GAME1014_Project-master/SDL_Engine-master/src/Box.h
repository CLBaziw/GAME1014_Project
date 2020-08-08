#pragma once
#ifndef __BOX__
#define __BOX__

#include "Obstacle.h"
#include "ObjectPool.h"

class Box
{
private:
	int m_x, m_y;
	Obstacle* m_obstacle;
	SDL_Rect m_rect;

	ObjectPool* m_objectPool;
public:
	Box(float x, float y);
	~Box();

	void Update();
	void Render();
	int GetX();
	Obstacle* GetSprite();

	Obstacle* GetRandomObstacle(ObjectPool* objPool, int x, int y);
};
#endif