#include "Box.h"
#include "Obstacle.h"
#include"Background.h"

#include <iostream>

#define SCROLLSPD 2

Box::Box(float x, float y)
{
	m_x = x;
	m_y = y;
	m_obstacle = nullptr;
}

Box::~Box()
{
	if (m_obstacle != nullptr)
	{
		delete m_obstacle;
		m_obstacle = nullptr;
	}
}

void Box::Update()
{
	m_x -= SCROLLSPD;

	if (m_obstacle != nullptr)
	{
		m_obstacle->getTransform()->position = glm::vec2(m_x, m_y);
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

Obstacle* Box::GetRandomObstacle(ObjectPool* objPool, int x, int y)
{
	int randNumber = (rand() % 4) + 1;

	Obstacle* sprite = nullptr;

	switch (randNumber)
	{
	case 1:
		std::cout << "Get Random Obstacle - Platform" << std::endl;
		sprite = objPool->GetObstacle(PLATFORM);
		sprite->getTransform()->position = glm::vec2(x, y);
		m_y = 420;
		break;
	case 2:
		std::cout << "Get Random Obstacle - Predator" << std::endl;
		sprite = objPool->GetObstacle(PREDATOR);
		sprite->getTransform()->position = glm::vec2(x, y);
		m_y = 524;
		break;
	case 3:
		std::cout << "Get Random Obstacle - Alien" << std::endl;
		sprite = objPool->GetObstacle(ENEMY);
		sprite->getTransform()->position = glm::vec2(x, y);
		m_y = 524;
		break;
	case 4:
		std::cout << "Get Random Obstacle - Obstacle1" << std::endl;
		sprite = objPool->GetObstacle(OBSTACLE1);
		sprite->getTransform()->position = glm::vec2(x, y);
		m_y = 578;
		break;
	case 5:
		std::cout << "Get Random Obstacle - Obstacle2" << std::endl;
		sprite = objPool->GetObstacle(OBSTACLE2);
		sprite->getTransform()->position = glm::vec2(x, y);
		break;
	case 6:
		std::cout << "Get Random Obstacle - Obstacle3" << std::endl;
		sprite = objPool->GetObstacle(OBSTACLE3);
		sprite->getTransform()->position = glm::vec2(x, y);
		break;
	}

	m_obstacle = sprite;
	return sprite;
}