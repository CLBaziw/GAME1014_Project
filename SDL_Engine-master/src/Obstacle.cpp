#include "Obstacle.h"
#include "TextureManager.h"
#include "Animation.h"
#include <iostream>

Obstacle::Obstacle(GameObjectType obsType)
{
	setType(obsType);

	if (getType() == ENEMY)
	{
		m_currentAnimationState = ENEMY_IDLE_LEFT;

		TextureManager::Instance()->loadSpriteSheet(
			"../Assets/sprites/alien.txt",
			"../Assets/sprites/alien.png",
			"alien"
		);

		setSpriteSheet(TextureManager::Instance()->getSpriteSheet("alien"));
	}
	else
	{
		m_currentAnimationState = NOT_ENEMY;

		TextureManager::Instance()->loadSpriteSheet(
			"../Assets/plat/plas.txt",
			"../Assets/plat/plas.png",
			"platform"
		);

		setSpriteSheet(TextureManager::Instance()->getSpriteSheet("platform"));
	}

	setActive(false);
	setWidth(128);
	setHeight(128);
	getTransform()->position = glm::vec2(300, 300);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;

	m_buildAnimations();
}

Obstacle::~Obstacle() = default;

void Obstacle::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	std::string sprite;
	if (getType() == ENEMY)
	{
		sprite = "alien";
	}
	else if (getType() == PLATFORM)
	{
		sprite = "platform";
	}

	TextureManager::Instance()->playAnimation(sprite, getAnimation("idle"), x, y, 0.12f, 0, 255, true);
}

void Obstacle::update()
{
	if (getType() == ENEMY)
	{
		// Update enemy
	}
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
	switch (getType())
	{
	/*case OBSTACLE1:

		break;
	case OBSTACLE2:

		break;
	case OBSTACLE3:

		break;*/
	case PLATFORM: {
		Animation platform = Animation();

		platform.name = "idle";
		platform.frames.push_back(getSpriteSheet()->getFrame("plas"));

		setAnimation(platform);
	}
		break;
	case ENEMY: {
		Animation idleAnimation = Animation();

		idleAnimation.name = "idle";
		idleAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-idle-0"));
		idleAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-idle-1"));
		idleAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-idle-2"));
		idleAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-idle-3"));

		setAnimation(idleAnimation);

		/*Animation runAnimation = Animation();

		runAnimation.name = "enemy-run";
		runAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-run-0"));
		runAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-run-1"));
		runAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-run-2"));
		runAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-run-3"));

		setAnimation(runAnimation);

		Animation deathAnimation = Animation();

		runAnimation.name = "enemy-death";
		runAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-death-0"));
		runAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-death-1"));
		runAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-death-2"));
		runAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-death-3"));

		setAnimation(deathAnimation);*/
	}
		break;	
	default:
		break;
	}
}
