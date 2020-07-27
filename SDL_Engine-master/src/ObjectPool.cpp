#include "ObjectPool.h"
#include <iostream>
#define MAXPLATFORM 4
#define MAXOBSTACLE 4
#define MAXENEMY 3
#define MAXPREDATOR 3


ObjectPool::ObjectPool()
{
	for (int i = 0; i < MAXPLATFORM; i++)
	{
		m_poolObstacles.push_back(new Obstacle(PLATFORM));
	}

	for (int i = 0; i < MAXOBSTACLE; i++)
	{
		m_poolObstacles.push_back(new Obstacle(OBSTACLE1));
		//m_poolObstacles.push_back(new Obstacle(OBSTACLE2));
		//m_poolObstacles.push_back(new Obstacle(OBSTACLE3));
	}

	for (int i = 0; i < MAXENEMY; i++)
	{
		m_poolObstacles.push_back(new Obstacle(ENEMY));
	}

	
	for (int i = 0; i < MAXPREDATOR; i++)
	{
		m_poolObstacles.push_back(new Obstacle(PREDATOR));
	}

}

ObjectPool::~ObjectPool() = default;

void ObjectPool::UpdateActiveSprites()
{
	for (int i = 0; i < m_poolObstacles.size(); i++)
	{
		if (m_poolObstacles[i]->getActive()) // Deactivate Inactive Sprites
		{
			m_poolObstacles[i]->update();
		}
	}
}

void ObjectPool::DrawActiveSprites()
{
	for (int i = 0; i < m_poolObstacles.size(); i++)
	{
		if (m_poolObstacles[i]->getActive()) // Deactivate Inactive Sprites
		{
			m_poolObstacles[i]->draw();
		}
	}
}

Obstacle* ObjectPool::GetObstacle(GameObjectType newObj)
{
	Obstacle* temp;

	for (int i = 0; i < m_poolObstacles.size(); i++)
	{
		if (m_poolObstacles[i]->getType() == newObj && !m_poolObstacles[i]->getActive())
		{
			temp = m_poolObstacles[i];
			temp->setActive(true);
			return temp;
		}
	}

	std::cout << "Nothing found" << std::endl;
}
