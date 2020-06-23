#include "Obstacle.h"
#include "TextureManager.h"

Obstacle::Obstacle(ObstacleType obsType)
{
	m_pObstacleType = obsType;
	// Check to see which obstacle was randomly chosen
	switch (m_pObstacleType)
	{
	case OBSTACLE1:
		// Set all necessary values (eg. texture, getTransform, etc.) depending on which obstacle is created
		break;
	case OBSTACLE2:

		break;
	case OBSTACLE3:

		break;
	default:
		break;
	}

	setType(OBSTACLE);

	m_buildAnimations();
}

Obstacle::~Obstacle() = default;

void Obstacle::draw()
{
	// copy and replace what is necessary from enemy or bullet
}

void Obstacle::update()
{
}

void Obstacle::clean()
{
}

void Obstacle::setPosition(int x, int y)
{
	getTransform()->position.x = x;
	getTransform()->position.y = y;
}

void Obstacle::m_buildAnimations()
{
}
