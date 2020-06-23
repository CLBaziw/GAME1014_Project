#include "Box.h"
#include "Platform.h"
#include "Enemy.h"

#include <iostream>

#define SCROLLSPD 2

Box::Box(int x, int y, bool hasSprite) : m_x(x), m_y(y), m_sprite(nullptr)
{
	if (hasSprite)
	{
		m_sprite = GetRandomObstacle(x, y);
		m_sprite->getTransform()->position = glm::vec2(x, y);
	}
}

Box::~Box()
{
	if (m_sprite != nullptr)
	{
		delete m_sprite;
		m_sprite = nullptr;
	}
}

void Box::Update()
{
	m_x -= SCROLLSPD;

	if (m_sprite != nullptr)
	{
		m_sprite->getTransform()->position = glm::vec2(m_x, m_sprite->getTransform()->position.y);
	}
}

void Box::Render()
{
}

int Box::GetX()
{
	return m_x;
}

Sprite* Box::GetSprite()
{
	return m_sprite;
}

Sprite* Box::GetRandomObstacle(int x, int y)
{
	int randNumber = (rand() % 2) + 1;

	Sprite* sprite = nullptr;

	switch (randNumber)
	{
	case 1:
		std::cout << "Get Random Obstacle - Platform" << std::endl;
		sprite = new Platform(x, 300);
		sprite->SetSafe(true);
		sprite->getTransform()->position = glm::vec2(x, 300);
		break;
	case 2:
		std::cout << "Get Random Obstacle - Enemy" << std::endl;
		sprite = new Enemy(x, 600);
		sprite->SetSafe(false);
		sprite->getTransform()->position = glm::vec2(x, 600);
		break;
	case 3:
		// obstacle type 1
		break;
	case 4:
		// obstacle type 2
		break;
	case 5:
		// obstacle type 3
		break;
	}

	return sprite;
}