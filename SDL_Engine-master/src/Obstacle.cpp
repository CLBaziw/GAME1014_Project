#include "Obstacle.h"
#include "TextureManager.h"

Obstacle::Obstacle(ObstacleType obsType)
{
	m_pObstacleType = obsType;
	// Check to see which obstacle was randomly chosen
	switch (m_pObstacleType)
	{
	case OBSTACLE1:
		TextureManager::Instance()->loadSpriteSheet(
			"../Assets/plat/platsprites.txt",
			"../Assets/plat/platsprites.png",
			"platsprites"
		);

		setSpriteSheet(TextureManager::Instance()->getSpriteSheet("platsprites"));

		setWidth(36);
		setHeight(58);

		getTransform()->position = glm::vec2(760.0f, 550.0f);
		getRigidBody()->isColliding = false;
		break;
	case OBSTACLE2:
		TextureManager::Instance()->loadSpriteSheet(
			"../Assets/sprites/Obstacles.txt",
			"../Assets/sprite/Fireball_Sheet.png",
			"Fireball_Sheet"
		);

		setSpriteSheet(TextureManager::Instance()->getSpriteSheet("Fireball_Sheet"));

		setWidth(36);
		setHeight(58);

		getTransform()->position = glm::vec2(760.0f, 550.0f);
		getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
		getRigidBody()->isColliding = false;
		break;
	case OBSTACLE3:
		TextureManager::Instance()->loadSpriteSheet(
			"../Assets/sprites/Obstacles.txt",
			"../Assets/sprites/SpikeEnemies.png",
			"SpikeEnemies"
		);

		setSpriteSheet(TextureManager::Instance()->getSpriteSheet("SpikeEnemies"));

		setWidth(36);
		setHeight(58);

		getTransform()->position = glm::vec2(760.0f, 550.0f);
		getRigidBody()->isColliding = false;
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
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	//draw the player according to animation state
	switch (m_pObstacleType)
	{
	case OBSTACLE1:
		TextureManager::Instance()->playAnimation("platsprites", getAnimation("hazard"),
			x, y, 0.12f, 0, 255, true);
		break;
	case OBSTACLE2:
		TextureManager::Instance()->playAnimation("Fireball_Sheet", getAnimation("Fireball"),
			x, y, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case OBSTACLE3:
		TextureManager::Instance()->playAnimation("SpikeEnemies", getAnimation("Spike-Enemy"),
			x, y, 0.25f, 0, 255, true);
		break;
	default:
		break;
	}
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
