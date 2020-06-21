#include "Player.h"
#include "TextureManager.h"
#include "BulletAnimationState.h"
#include <algorithm>

Player::Player() : m_currentAnimationState(PLAYER_IDLE_RIGHT)
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/resources.txt",
		"../Assets/sprites/resources.png",
		"resources");

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("resources"));

	// set frame width
	setWidth(77);

	// set frame height
	setHeight(70);



	getTransform()->position = glm::vec2(380.0f, 400.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;

	m_shooting = false;
	m_jumping = false;
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	m_maxVelX = 300.0;
	m_maxVelY = JUMPFORCE;
	m_grav = GRAV;
	m_drag = 0.88;

	setType(PLAYER);

	m_buildAnimations();
}

Player::~Player()
= default;

void Player::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;


	// draw the player according to animation state
	switch (m_currentAnimationState)
	{
	case PLAYER_IDLE_RIGHT:
		TextureManager::Instance()->playAnimation("resources", getAnimation("idle"),
			x, y, 0.12f, 0, 255, true);
		break;
	case PLAYER_IDLE_LEFT:
		TextureManager::Instance()->playAnimation("resources", getAnimation("idle"),
			x, y, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case PLAYER_RUN_RIGHT:
		TextureManager::Instance()->playAnimation("resources", getAnimation("run"),
			x, y, 0.25f, 0, 255, true);
		break;
	case PLAYER_RUN_LEFT:
		TextureManager::Instance()->playAnimation("resources", getAnimation("run"),
			x, y, 0.25f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	default:
		break;
	}

}

void Player::update()
{

	// check jumping on x-axis
	m_velX += m_accelX;
	m_velX *= (m_jumping ? m_drag : 1);
	m_velX = std::min(std::max(m_velX, -(m_maxVelX)), (m_maxVelX));
	getTransform()->position.x += (int)m_velX; // Had to cast it to int to get crisp collision with side of platform.
	// check jumping on y-axis
	m_velY += m_accelY + m_grav; // Adjust gravity to get slower jump.
	m_velY = std::min(std::max(m_velY, -(m_maxVelY)), (m_grav * 5));
	getTransform()->position.y += (int)m_velY; // To remove aliasing, I made cast it to an int too.
	m_accelX = m_accelY = 0.0;
}

void Player::clean()
{
}



void Player::setAnimationState(const PlayerAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Player::setPosition(float x, float y)
{
	getTransform()->position.x = x;
	getTransform()->position.y = y;
}


void Player::Stop() // stop at the right time
{
	StopX();
	StopY();
}
void Player::StopX() { m_velX = 0.0; }
void Player::StopY() { m_velY = 0.0; }

void Player::SetAccelX(double a) { m_accelX = a; }
void Player::SetAccelY(double a) { m_accelY = a; }
bool Player::isJumping() { return m_jumping; }
void Player::SetJumping(bool j) { m_jumping = j; }
double Player::GetVelX() { return m_velX; }
double Player::GetVelY() { return m_velY; }

void Player::SetX(float x)
{
	//getTransform()->position.x = x;
	getTransform()->position.x = x;
}

void Player::SetY(float y)
{
	//getTransform()->position.y = y;
	getTransform()->position.y = y;
}

bool Player::isShooting() { return m_shooting; }
void Player::SetShooting(bool s) { m_shooting = s; }

void Player::setDst(/*float x, float y, float w, float h*/)
{
	//getTransform()->position.x = x;
	dst.x = getTransform()->position.x;	
	
	//getTransform()->position.y = y;
	dst.y = getTransform()->position.y;

	dst.w = getWidth();

	dst.h = getHeight();
}

SDL_FRect& Player::getDst()
{
	return dst;
}

void Player::m_buildAnimations()
{
	Animation idleAnimation = Animation();

	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("astro-stand-0"));

	setAnimation(idleAnimation);

	Animation runAnimation = Animation();

	runAnimation.name = "run";
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("astro-run-0"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("astro-run-1"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("astro-run-2"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("astro-run-3"));

	setAnimation(runAnimation);
}
