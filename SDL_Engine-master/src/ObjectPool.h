#pragma once
#ifndef __OBJECT_POOL__
#define __OBJECT_POOL__

#include "GameObject.h"
#include "GameObjectType.h"
#include "Obstacle.h"
#include <vector>


class ObjectPool
{
public:
	ObjectPool();
	~ObjectPool();

	void UpdateActiveSprites();
	void DrawActiveSprites();
	Obstacle* GetObstacle(GameObjectType newObj);
private:
	std::vector<Obstacle*> m_activeObstacles;
	std::vector<Obstacle*> m_poolObstacles;
};

#endif

