#include "ObjectPool.h"
#include <iostream>

#define MAXPLATFORM 5
#define MAXOBSTACLE 5
#define MAXENEMY 6

ObjectPool::ObjectPool()
{
	m_activeObstacles.reserve(5);
	for (int i = 0; i < MAXPLATFORM; i++)
	{
		m_poolObstacles.push_back(new Obstacle(PLATFORM));
	}

	for (int i = 0; i < MAXOBSTACLE; i++)
	{
		m_poolObstacles.push_back(new Obstacle(OBSTACLE1));
		m_poolObstacles.push_back(new Obstacle(OBSTACLE2));
		m_poolObstacles.push_back(new Obstacle(OBSTACLE3));
	}

	for (int i = 0; i < MAXENEMY; i++)
	{
		m_poolObstacles.push_back(new Obstacle(ENEMY));
	}
}

ObjectPool::~ObjectPool()
{
}

void ObjectPool::UpdateActiveSprites()
{
	for (int i = 0; i < m_activeObstacles.size(); i++)
	{
		//std::cout << "Position: " << i << " Type: " << m_activeObstacles[i]->getType() << " Active: " << m_activeObstacles[i]->getActive() << std::endl;

		if (m_activeObstacles[i]->getActive()) // Deactivate Inactive Sprites
		{
			m_activeObstacles[i]->update();
		}
		else
		{
			std::cout << "Remove inactive sprite from list of active sprites" << std::endl;
			m_activeObstacles.erase(m_activeObstacles.begin() + i);
		}
	}
}

void ObjectPool::DrawActiveSprites()
{
	for (int i = 0; i < m_activeObstacles.size(); i++)
	{
		//std::cout << "Position: " << i << " Type: " << m_activeObstacles[i]->getType() << " Active: " << m_activeObstacles[i]->getActive() << std::endl;

		if (m_activeObstacles[i]->getActive()) // Deactivate Inactive Sprites
		{
			m_activeObstacles[i]->draw();
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
			m_activeObstacles.push_back(temp);
			return temp;
		}
	}

	std::cout << "Nothing found" << std::endl;
}
