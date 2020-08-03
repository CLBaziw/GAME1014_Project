#include "EnemyBullet.h"
#include "TextureManager.h"
#include <iostream>
#include "../src/TextureManager.h"
#include "../src/Sprite.h"



#define BULLETSPEED 20;

EnemyBullet::EnemyBullet(float xPos, float yPos, bool bulletType, EnemyBulletAnimationState bulletState)
{
	m_EnemyBullet = bulletType; // If we decide different enemies will have different colour/shape bullets then we will need to make an enum
	if (m_EnemyBullet)
	{
		std::cout << "Enemy bullet created " << std::endl;
		TextureManager::Instance()->loadSpriteSheet(
			"../Assets/sprites/Ebullet.txt",
			"../Assets/sprites/Ebullet.png",
			"Enemy-bullet-sprite"
		);

		setSpriteSheet(TextureManager::Instance()->getSpriteSheet("Enemy-bullet-sprite"));
		setType(E_BULLET);
	}
	else
	{

		setType(P_BULLET);
	}

	setWidth(20);
	setHeight(10);

	getTransform()->position = glm::vec2(xPos, yPos);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;

	setAnimationState(bulletState);

	if (m_currentAnimationState == ENEMY_BULLET_MOVE_RIGHT)
	{
		m_speed = BULLETSPEED;
	}
	else if (m_currentAnimationState == ENEMY_BULLET_MOVE_LEFT)
	{
		m_speed = -BULLETSPEED;
	}

	m_buildAnimations();
}

EnemyBullet::~EnemyBullet() = default;

void EnemyBullet::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// Check if bullet is player's or enemy's to change appearance of bullet
	std::string sprite;
	if (m_EnemyBullet) {
		sprite = "Enemy-bullet-sprite";
	}
	else {
		sprite = "player-bullet-sprite";
	}

	//draw the bullet according to animation state
	switch (m_currentAnimationState)
	{
	case ENEMY_BULLET_MOVE_RIGHT:
		TextureManager::Instance()->playAnimation(sprite, getAnimation("Ebullet-moving"),
			x, y, 0.12f, 0, 255, true);
		break;
	case ENEMY_BULLET_MOVE_LEFT:
		TextureManager::Instance()->playAnimation(sprite, getAnimation("Ebullet-moving"),
			x, y, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case ENEMY_BULLET_CRASH_RIGHT:
		TextureManager::Instance()->playAnimation(sprite, getAnimation("Ebullet-colliding"),
			x, y, 0.25f, 0, 255, true);
		break;
	case ENEMY_BULLET_CRASH_LEFT:
		TextureManager::Instance()->playAnimation(sprite, getAnimation("Ebullet-colliding"),
			x, y, 0.25f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	default:
		break;
	}
}

void EnemyBullet::update()
{
	getTransform()->position.x += m_speed;
	getTransform()->position.x -= m_speed;
}

void EnemyBullet::clean()
{
}

void EnemyBullet::setAnimationState(EnemyBulletAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void EnemyBullet::setPosition(int x, int y)
{
	getTransform()->position.x = x + 40;

	getTransform()->position.y = y + 23;
}

void EnemyBullet::m_buildAnimations()
{
	Animation bMovingAnimation = Animation();

	bMovingAnimation.name = "Ebullet-moving";
	bMovingAnimation.frames.push_back(getSpriteSheet()->getFrame("Ebullet-moving-0"));
	/*bMovingAnimation.frames.push_back(getSpriteSheet()->getFrame("Ebullet-moving-1"));
	bMovingAnimation.frames.push_back(getSpriteSheet()->getFrame("Ebullet-moving-2"));
	bMovingAnimation.frames.push_back(getSpriteSheet()->getFrame("Ebullet-moving-3"));*/

	setAnimation(bMovingAnimation);

	Animation bCollidingAnimation = Animation();

	bCollidingAnimation.name = "Ebullet-colliding";
	bCollidingAnimation.frames.push_back(getSpriteSheet()->getFrame("Ebullet-colliding-0"));
	/*bCollidingAnimation.frames.push_back(getSpriteSheet()->getFrame("Ebullet-colliding-1"));
	bCollidingAnimation.frames.push_back(getSpriteSheet()->getFrame("Ebullet-colliding-2"));
	bCollidingAnimation.frames.push_back(getSpriteSheet()->getFrame("Ebullet-colliding-3"));*/

	setAnimation(bCollidingAnimation);
}
