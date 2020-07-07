#include "Enemy.h"
#include "EnemyAnimationState.h"
#include "TextureManager.h"
#include <algorithm>
#include "Player.h"
#include "EnemyBullet.h"

Enemy::Enemy()
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/alien.txt",
		"../Assets/sprites/alien.png",
		"alien"
	);

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("alien"));
	
	setWidth(128);
	setHeight(128);

	m_shooting = false;
	getTransform()->position = glm::vec2(760.0f, 525.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;


	setAnimationState(ENEMY_IDLE_LEFT);
	setType(ENEMY);

	m_buildAnimations();
}

Enemy::~Enemy() = default;

void Enemy::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	//draw the player according to animation state
	switch (m_currentAnimationState)
	{
	case ENEMY_IDLE_RIGHT:
		TextureManager::Instance()->playAnimation("alien", getAnimation("enemy-idle"),
			x, y, 0.12f, 0, 255, true);
		break;
	case ENEMY_IDLE_LEFT:
		TextureManager::Instance()->playAnimation("alien", getAnimation("enemy-idle"),
			x, y, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case ENEMY_RUN_RIGHT:
		TextureManager::Instance()->playAnimation("alien", getAnimation("enemy-run"),
			x, y, 0.25f, 0, 255, true);
		break;
	case ENEMY_RUN_LEFT:
		TextureManager::Instance()->playAnimation("alien", getAnimation("enemy-run"),
			x, y, 0.25f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	/*case ENEMY_DIE_RIGHT:
		TextureManager::Instance()->playAnimation("enemy-sprite", getAnimation("enemy-dead"),
			x, y, 0.30f, 0, 255, true);
		break;
	case ENEMY_DIE_LEFT:
		TextureManager::Instance()->playAnimation("enemy-sprite", getAnimation("enemy-dead"),
			x, y, 0.30f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;*/
	default:
		break;
	}
}

void Enemy::update()
{

}

void Enemy::clean()
{
}

void Enemy::setAnimationState(EnemyAnimationState new_state)
{
}

void Enemy::setAnimationState(EnemyAnimationState new_state)
{
	m_currentAnimationState = new_state;
}
//
//
//
void Enemy::setPosition(int x, int y)
{
	getTransform()->position.x = x ;
	getTransform()->position.y = y;
}
//
//
//void Enemy::SetX(float x)
//{
//	//getTransform()->m_dst.x = x;
//	getTransform()->position.x = x + 70;
//}
//
//void Enemy::SetY(float y)
//{
//	//getTransform()->m_dst.y = y;
//	getTransform()->position.y = y + 70;
//}
//
//bool Enemy::isShooting() { return m_shooting; }
//void Enemy::SetShooting(bool s) { m_shooting = s; }
//
//void Enemy::setDst()
//{
//	dst.x = getTransform()->position.x;
//
//	dst.y = getTransform()->position.y;
//
//	dst.w = getWidth();
//
//	dst.h = getHeight();
//}
//
//SDL_FRect& Enemy::getDst()
//{
//	return dst;
//}

void Enemy::m_buildAnimations()
{
	Animation idleAnimation = Animation();

	idleAnimation.name = "enemy-idle";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-idle-0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-idle-1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-idle-2"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-idle-3"));

	setAnimation(idleAnimation);

	Animation runAnimation = Animation();

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

	setAnimation(deathAnimation);
}