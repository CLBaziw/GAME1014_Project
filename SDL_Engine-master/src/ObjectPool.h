#pragma once
#ifndef __OBJECT_POOL__
#define __OBJECT_POOL__

#include "GameObject.h"
#include "GameObjectType.h"
#include "Obstacle.h"
#include "Bullet.h"
#include <vector>


class ObjectPool
{
public:
	ObjectPool();
	~ObjectPool();

	void UpdateActiveSprites();
	void DrawActiveSprites();
	Obstacle* GetObstacle(GameObjectType newObj);
	Bullet* GetBullet(BulletType bullType);
private:
	std::vector<Obstacle*> m_poolObstacles;
	std::vector<Bullet*> m_poolPlayerBullets;
	std::vector<Bullet*> m_poolEnemyBullets;
};

#endif

