#include "Enemy.h"
#include "TextureManager.h"

Enemy::Enemy()
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/resources.txt",
		"../Assets/sprites/resources.png",
		"resources");

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("resources"));

	// set frame width
	setWidth(65);

	// set frame height
	setHeight(65);

	getTransform()->position = glm::vec2(550.0f, 550.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLANE);

	m_buildAnimations();
}

Enemy::~Enemy() = default;

void Enemy::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the plane sprite with simple propeller animation
	TextureManager::Instance()->playAnimation("resources", getAnimation("alien"), x, y, 0.5f, 0, 255, true, SDL_FLIP_HORIZONTAL);
}

void Enemy::update()
{
}

void Enemy::clean()
{
}

void Enemy::m_buildAnimations()
{
	Animation ailenAnimation = Animation();

	ailenAnimation.name = "alien";
	ailenAnimation.frames.push_back(getSpriteSheet()->getFrame("alien-0"));
	ailenAnimation.frames.push_back(getSpriteSheet()->getFrame("alien-1"));
	ailenAnimation.frames.push_back(getSpriteSheet()->getFrame("alien-2"));
	ailenAnimation.frames.push_back(getSpriteSheet()->getFrame("alien-3"));

	setAnimation(ailenAnimation);
}
