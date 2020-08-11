#include "Box.h"
#include "Obstacle.h"
#include"Background.h"
#include "Game.h"

#include <iostream>
#include <fstream>

#define SCROLLSPD 2.6

#define PLATFORMY 500
#define ENEMYY 715
#define SPIKEY 725

Box::Box(float x, float y)
{
	
	m_x = x;
	m_y = y;
	m_obstacle = nullptr;
}

Box::~Box()
{
}

void Box::Update()
{
	m_x -= SCROLLSPD;
	int windowHeight = TheGame::Instance()->getWindowHeight();
	if (m_obstacle != nullptr)
	{
		if (m_obstacle->getType() != PLATFORM)
		{
			m_obstacle->getTransform()->position = glm::vec2(m_x, windowHeight / 2 + 204);
		}
		else
		{
			m_obstacle->getTransform()->position = glm::vec2(m_x, windowHeight / 2 + 100);
		}
		
	}
}

void Box::Render()
{
}

int Box::GetX()
{
	return m_x;
}

Obstacle* Box::GetSprite()
{
	return m_obstacle;
}

Obstacle* Box::GetObstacle(ObjectPool* objPool, int x, int y, int level, int obsNum, std::vector<int> obs)
{
	int num;

	if (level < 2) // Level 3 is infinite with random spawning obstacles
	{
		num = obs[obsNum];
	}
	else 
	{
		num = (rand() % 8) + 1;
	}

	Obstacle* sprite = nullptr;

	switch (num)
	{
	case 1:
		std::cout << "Get Random Obstacle - Platform" << std::endl;
		sprite = objPool->GetObstacle(PLATFORM);
		m_y = PLATFORMY;
		sprite->getTransform()->position = glm::vec2(x, m_y);
		break;
	case 2:
		std::cout << "Get Random Obstacle - Predator" << std::endl;
		sprite = objPool->GetObstacle(PREDATOR);
		m_y = ENEMYY;
		sprite->getTransform()->position = glm::vec2(x, m_y);
		break;
	case 3:
		std::cout << "Get Random Obstacle - Alien" << std::endl;
		sprite = objPool->GetObstacle(ENEMY);
		m_y = ENEMYY;
		sprite->getTransform()->position = glm::vec2(x, m_y);
		break;
	case 4:
	case 8:
		std::cout << "Get Random Obstacle - Obstacle1" << std::endl;
		sprite = objPool->GetObstacle(OBSTACLE1);
		m_y = SPIKEY;
		sprite->getTransform()->position = glm::vec2(x, m_y);
		break;
	case 5:
		std::cout << "Get Random Obstacle - Obstacle4" << std::endl;
		sprite = objPool->GetObstacle(OBSTACLE4);
		m_y = SPIKEY;
		sprite->getTransform()->position = glm::vec2(x, m_y);
		break;
	case 6:
		std::cout << "Get Random Obstacle - Obstacle5" << std::endl;
		sprite = objPool->GetObstacle(OBSTACLE5);
		m_y = SPIKEY;
		sprite->getTransform()->position = glm::vec2(x, m_y);
		break;
	case 7:
		std::cout << "Get Random Obstacle - Obstacle6" << std::endl;
		sprite = objPool->GetObstacle(OBSTACLE6);
		m_y = SPIKEY;
		sprite->getTransform()->position = glm::vec2(x, m_y);
		break;
	}
	m_obstacle = sprite;
	return sprite;
}