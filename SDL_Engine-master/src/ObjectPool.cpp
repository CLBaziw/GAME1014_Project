#include "ObjectPool.h"
#include <iostream>
#define MAXPLATFORM 4
#define MAXOBSTACLE 9
#define MAXENEMY 3
#define MAXPREDATOR 3
#define MAXBULLETS 100


ObjectPool::ObjectPool()
{
	for (int i = 0; i < MAXPLATFORM; i++)
	{
		m_poolObstacles.push_back(new Obstacle(PLATFORM));
	}

	for (int i = 0; i < 1; i++)
	{
		m_poolObstacles.push_back(new Obstacle(OBSTACLE1));
		//m_poolObstacles.push_back(new Obstacle(OBSTACLE2));
		//m_poolObstacles.push_back(new Obstacle(OBSTACLE3));
		m_poolObstacles.push_back(new Obstacle(OBSTACLE4));
		m_poolObstacles.push_back(new Obstacle(OBSTACLE5));
		m_poolObstacles.push_back(new Obstacle(OBSTACLE6));

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
		if (i < 24)
		{
			m_poolPlayerBullets.push_back(new Bullet(0, 0, PLAYER_BULLET, P_BULLET_MOVE_RIGHT));
		}
		else
		{
			m_poolPlayerBullets.push_back(new Bullet(0, 0, PLAYER_BULLET2, P2_BULLET_MOVE_RIGHT));
		}

		m_poolEnemyBullets.push_back(new Bullet(0, 0, ENEMY_BULLET, E_BULLET_MOVE_RIGHT));
		m_poolEnemyBullets.push_back(new Bullet(0, 0, ENEMY_BULLET2, E2_BULLET_MOVE_RIGHT));
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

void ObjectPool::DisplayActiveList()
{
	int plat = 0;
	int obs = 0;
	int enemy = 0;
	int pred = 0;
	int bulletP = 0;
	int bulletP2 = 0;
	int bulletE = 0;
	int bulletE2 = 0;

	for (int i = 0; i < m_poolObstacles.size(); i++)
	{
		if (m_poolObstacles[i]->getActive())
		{
			switch (m_poolObstacles[i]->getType())
			{
			case PLATFORM:
				plat++;
				break;
			case OBSTACLE1:
				obs++;
				break;
			case OBSTACLE2:
				obs++;
				break;
			case OBSTACLE3:
				obs++;
				break;
			case OBSTACLE4:
				obs++;
				break;
			case OBSTACLE5:
				obs++;
				break;
			case OBSTACLE6:
				obs++;
				break;
			case ENEMY:
				enemy++;
				break;
			case PREDATOR:
				pred++;
				break;
			}
		}
	}

	for (int i = 0; i < m_poolPlayerBullets.size(); i++)
	{
		if (m_poolPlayerBullets[i]->getActive())
		{
			if (m_poolPlayerBullets[i]->getType() == P_BULLET)
			{
				bulletP++;
			}
			if (m_poolPlayerBullets[i]->getType() == P2_BULLET)
			{
				bulletP2++;
			}
		}
	}

	for (int i = 0; i < m_poolEnemyBullets.size(); i++)
	{
		if (m_poolEnemyBullets[i]->getActive())
		{
			if (m_poolEnemyBullets[i]->getType() == E_BULLET)
			{
				bulletE++;
			}
			
			if (m_poolEnemyBullets[i]->getType() == E2_BULLET)
			{
				bulletE2++;
			}
		}
	}

	std::cout << "Platform: " << plat << std::endl;
	std::cout << "Obstacle: " << obs << std::endl;
	std::cout << "Enemy: " << enemy << std::endl;
	std::cout << "Predator " << pred << std::endl;
	std::cout << "Player Bullet 1: " << bulletP << std::endl;
	std::cout << "Player Bullet 2: " << bulletP2 << std::endl;
	std::cout << "Enemy Bullet: " << bulletE << std::endl;
	std::cout << "Enemy Bullet 2: " << bulletE2 << std::endl;
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
			if (!m_poolPlayerBullets[i]->getActive() && m_poolPlayerBullets[i]->getType() == P_BULLET)
			{
				temp = m_poolPlayerBullets[i];
				temp->setActive(true);
				return temp;
			}
		}
		else if (bullType == PLAYER_BULLET2)
		{
			if (!m_poolPlayerBullets[i]->getActive() && m_poolPlayerBullets[i]->getType() == P2_BULLET)
			{
				temp = m_poolPlayerBullets[i];
				temp->setActive(true);
				return temp;
			}
		}
		else if (bullType == ENEMY_BULLET )
		{
			if (!m_poolEnemyBullets[i]->getActive() && m_poolEnemyBullets[i]->getType() == E_BULLET)
			{
				temp = m_poolEnemyBullets[i];
				temp->setActive(true);
				return temp;
			}
		}
		else if (bullType == ENEMY_BULLET2)
		{
			if (!m_poolEnemyBullets[i]->getActive() && m_poolEnemyBullets[i]->getType() == E2_BULLET)
			{
				temp = m_poolEnemyBullets[i];
				temp->setActive(true);
				return temp;
			}
		}
	}
}
