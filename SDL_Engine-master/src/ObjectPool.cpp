#include "ObjectPool.h"
#include <iostream>
#define MAXPLATFORM 4
#define MAXOBSTACLE 4
#define MAXENEMY 3
#define MAXPREDATOR 3
#define MAXBULLETS 30


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

	for (int i = 0; i < MAXBULLETS; i++)
	{
		m_poolPlayerBullets.push_back(new Bullet(0, 0, PLAYER_BULLET, P_BULLET_MOVE_RIGHT));
		m_poolEnemyBullets.push_back(new Bullet(0, 0, ENEMY_BULLET, E_BULLET_MOVE_RIGHT));
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

	for (int i = 0; i < MAXBULLETS; i++) 
	{
		if (m_poolPlayerBullets[i]->getActive())
		{
			m_poolPlayerBullets[i]->update();
		}

		if (m_poolEnemyBullets[i]->getActive())
		{
			m_poolEnemyBullets[i]->update();
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

	for (int i = 0; i < MAXBULLETS; i++)
	{
		if (m_poolPlayerBullets[i]->getActive())
		{
			m_poolPlayerBullets[i]->draw();
		}

		if (m_poolEnemyBullets[i]->getActive())
		{
			m_poolEnemyBullets[i]->draw();
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

Bullet* ObjectPool::GetBullet(BulletType bullType)
{
	Bullet* temp;

	for (int i = 0; i < MAXBULLETS; i++)
	{
		if (bullType == PLAYER_BULLET)
		{
			if (!m_poolPlayerBullets[i]->getActive())
			{
				temp = m_poolPlayerBullets[i];
				temp->setActive(true);
				return temp;
			}
		}
		else if (bullType == ENEMY_BULLET)
		{
			if (!m_poolEnemyBullets[i]->getActive())
			{
				temp = m_poolEnemyBullets[i];
				temp->setActive(true);
				return temp;
			}
		}
	}
}
