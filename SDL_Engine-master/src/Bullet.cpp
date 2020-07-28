#include "Bullet.h"
#include "TextureManager.h"

#define BULLETSPEED 10;
#define EBULLETSPEED 5;

Bullet::Bullet(float xPos, float yPos, /*bool bulletType*/BulletType bulletType, BulletAnimationState bulletState)
{
	//m_playerBullet = bulletType; // If we decide different enemies will have different colour/shape bullets then we will need to make an enum
	this->m_BulletType = bulletType;
	switch(m_BulletType)//if (m_playerBullet)
	{
		case PLAYER_BULLET:
		{
			std::cout << "Player shoot laser" << std::endl;
			TextureManager::Instance()->loadSpriteSheet(
				"../Assets/sprites/beams.txt",
				"../Assets/sprites/beams.png",
				"player-bullet-sprite"
			);

			setSpriteSheet(TextureManager::Instance()->getSpriteSheet("player-bullet-sprite"));
			setType(P_BULLET);

			setWidth(20);
			setHeight(10);

			setAnimationState(bulletState);
			if (m_currentAnimationState == P_BULLET_MOVE_RIGHT)
			{
				m_speed = BULLETSPEED;
			}
			else if (m_currentAnimationState == P_BULLET_MOVE_LEFT)
			{
				m_speed = -BULLETSPEED;
			}
			break;
		}
		case PLAYER_BULLET2:
		{
			std::cout << "Player shoot fire" << std::endl;
			TextureManager::Instance()->loadSpriteSheet(
				"../Assets/sprites/Fireball.txt",
				"../Assets/sprites/Fireball.png",
				"Fireball"
			);
			setSpriteSheet(TextureManager::Instance()->getSpriteSheet("Fireball"));
			setType(P_BULLET);

			setWidth(46);
			setHeight(46);


			setAnimationState(bulletState);
			if (m_currentAnimationState == P2_BULLET_MOVE_RIGHT)
			{
				m_speed = BULLETSPEED;
			}
			else if (m_currentAnimationState == P2_BULLET_MOVE_LEFT)
			{
				m_speed = -BULLETSPEED;
			}
			break;
			
		}
		case ENEMY_BULLET://else
		{
			// Don't have enemy bullet sprite made yet but this would be same code as above but for enemy bullet sprite
			std::cout << "Enemy bullet created " << std::endl;
			TextureManager::Instance()->loadSpriteSheet(
				"../Assets/sprites/enemybullet.txt",
				"../Assets/sprites/enemybullet.png",
				"enemy-bullet-sprite"
			);

			setSpriteSheet(TextureManager::Instance()->getSpriteSheet("enemy-bullet-sprite"));
			setType(E_BULLET);

			setWidth(70);
			setHeight(41);


			setAnimationState(bulletState);
			if (m_currentAnimationState == E_BULLET_MOVE_RIGHT)
			{
				m_speed = EBULLETSPEED;
			}
			else if (m_currentAnimationState == E_BULLET_MOVE_LEFT)
			{
				m_speed = -EBULLETSPEED;
			}
			break;
		}
		default:
			break;
	}


	getTransform()->position = glm::vec2(xPos + 30, yPos + 27);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;

	setActive(false);

	m_buildAnimations();
}

Bullet::~Bullet() = default;

void Bullet::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// Check if bullet is player's or enemy's to change appearance of bullet


	switch (m_BulletType)
	{
		case PLAYER_BULLET:
		{
			switch (m_currentAnimationState)
			{
				case P_BULLET_MOVE_RIGHT:
				{
					TextureManager::Instance()->playAnimation("player-bullet-sprite", getAnimation("p_bullet-moving"),
						x, y, 0.12f, 0, 200, true);
					break;

				}
				case P_BULLET_MOVE_LEFT:
				{
					TextureManager::Instance()->playAnimation("player-bullet-sprite", getAnimation("p_bullet-moving"),
						x, y, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
					break;
				}
			}
			break;			
		}
		case PLAYER_BULLET2:
		{
			switch (m_currentAnimationState)
			{
				case P2_BULLET_MOVE_RIGHT:
				{
					TextureManager::Instance()->playAnimation("Fireball", getAnimation("p2_bullet-moving"),
						x, y, 0.12f, 0, 200, true);
					break;

				}
				case P2_BULLET_MOVE_LEFT:
				{
					TextureManager::Instance()->playAnimation("Fireball", getAnimation("p2_bullet-moving"),
						x, y, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
					break;
				}
			}
			break;
		}
		case ENEMY_BULLET:
		{
			switch (m_currentAnimationState)
			{
				case E_BULLET_MOVE_RIGHT:
				{
					TextureManager::Instance()->playAnimation("enemy-bullet-sprite", getAnimation("e_bullet-moving"),
						x, y, 0.12f, 0, 200, true);
					break;

				}
				case E_BULLET_MOVE_LEFT:
				{
					TextureManager::Instance()->playAnimation("enemy-bullet-sprite", getAnimation("e_bullet-moving"),
						x, y, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
					break;
				}
			}
			break;		
		}
		default:
			break;
	}

	//draw the bullet according to animation state

}

void Bullet::update()
{
	if (m_currentAnimationState == P_BULLET_MOVE_RIGHT || m_currentAnimationState == E_BULLET_MOVE_RIGHT)
	{
		getTransform()->position.x += m_speed;
	}
	else
	{
		getTransform()->position.x -= m_speed;
	}
	
}

void Bullet::clean()
{
}

void Bullet::setAnimationState(BulletAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Bullet::setPosition(int x, int y)
{
	getTransform()->position.x = x;
	getTransform()->position.y = y;
}

void Bullet::m_buildAnimations()
{
	switch(m_BulletType)
	{
		case PLAYER_BULLET:
			{
				Animation bMovingAnimation = Animation();

				bMovingAnimation.name = "p_bullet-moving";
				bMovingAnimation.frames.push_back(getSpriteSheet()->getFrame("bullet-moving-0"));


				setAnimation(bMovingAnimation);

				Animation bCollidingAnimation = Animation();

				bCollidingAnimation.name = "p_bullet-colliding";
				bCollidingAnimation.frames.push_back(getSpriteSheet()->getFrame("bullet-colliding-0"));


				setAnimation(bCollidingAnimation);
				break;
			}
		case PLAYER_BULLET2:
			{
				Animation b2MovingAnimation = Animation();

				b2MovingAnimation.name = "p2_bullet-moving";
				b2MovingAnimation.frames.push_back(getSpriteSheet()->getFrame("fire-0"));


				setAnimation(b2MovingAnimation);
				break;
			}
		case ENEMY_BULLET:
			{
				Animation bMovingAnimation = Animation();

				bMovingAnimation.name = "e_bullet-moving";
				bMovingAnimation.frames.push_back(getSpriteSheet()->getFrame("bullet-moving-0"));

				setAnimation(bMovingAnimation);

				Animation bCollidingAnimation = Animation();

				bCollidingAnimation.name = "e_bullet-colliding";
				bCollidingAnimation.frames.push_back(getSpriteSheet()->getFrame("bullet-colliding-0"));

				setAnimation(bCollidingAnimation);
				break;
			}
		}
}
