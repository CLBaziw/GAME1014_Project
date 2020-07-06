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
			"../Assets/sprites/enemy_small.txt",
			"../Assets/sprites/enemy_small.png",
			"enemy-sprite"
		);

		setSpriteSheet(TextureManager::Instance()->getSpriteSheet("enemy-sprite"));

		setWidth(36);
		setHeight(58);

		getTransform()->position = glm::vec2(760.0f, 550.0f);
		getRigidBody()->isColliding = false;
		break;
	case OBSTACLE2:
		TextureManager::Instance()->loadSpriteSheet(
			"../Assets/sprites/enemy_small.txt",
			"../Assets/sprites/enemy_small.png",
			"enemy-sprite"
		);

		setSpriteSheet(TextureManager::Instance()->getSpriteSheet("enemy-sprite"));

		setWidth(36);
		setHeight(58);

		getTransform()->position = glm::vec2(760.0f, 550.0f);
		getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
		getRigidBody()->isColliding = false;
		break;
	case OBSTACLE3:
		TextureManager::Instance()->loadSpriteSheet(
			"../Assets/sprites/enemy_small.txt",
			"../Assets/sprites/enemy_small.png",
			"enemy-sprite"
		);

		setSpriteSheet(TextureManager::Instance()->getSpriteSheet("enemy-sprite"));

		setWidth(36);
		setHeight(58);

		getTransform()->position = glm::vec2(760.0f, 550.0f);
		getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
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
		TextureManager::Instance()->playAnimation("enemy-sprite", getAnimation("enemy-idle"),
			x, y, 0.12f, 0, 255, true);
		break;
	case OBSTACLE2:
		TextureManager::Instance()->playAnimation("enemy-sprite", getAnimation("enemy-idle"),
			x, y, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case OBSTACLE3:
		TextureManager::Instance()->playAnimation("enemy-sprite", getAnimation("enemy-run"),
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
