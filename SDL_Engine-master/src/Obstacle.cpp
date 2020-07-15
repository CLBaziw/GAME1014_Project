#include "Obstacle.h"
#include "TextureManager.h"
#include "Animation.h"
#include "Game.h"


Obstacle::Obstacle(GameObjectType obsType)
{
	setType(obsType);
	// Check to see which obstacle was randomly chosen
	switch (getType())
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
		m_currentAnimationState = NOT_ENEMY;

		getTransform()->position = glm::vec2(760.0f, 550.0f);
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
		m_currentAnimationState = NOT_ENEMY;

		getTransform()->position = glm::vec2(760.0f, 550.0f);
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
		m_currentAnimationState = NOT_ENEMY;

		getTransform()->position = glm::vec2(760.0f, 550.0f);
		break;
		
	case PLATFORM:
		// Copy from obstacle but make relevant for platform
		TextureManager::Instance()->loadSpriteSheet(
			"../Assets/plat/plas.txt",
			"../Assets/plat/plas.png",
			"platform"
		);

		setSpriteSheet(TextureManager::Instance()->getSpriteSheet("platform"));

		setWidth(128);
		setHeight(128);
		m_currentAnimationState = NOT_ENEMY;

		getTransform()->position = glm::vec2(760.0f, 550.0f);

		break;
	
	/*case ENEMY:
		TextureManager::Instance()->loadSpriteSheet(
			"../Assets/sprites/alien.txt",
			"../Assets/sprites/alien.png",
			"alien"
		);
		setSpriteSheet(TextureManager::Instance()->getSpriteSheet("alien"));

		setWidth(128);
		setHeight(128);
		m_currentAnimationState = ENEMY_IDLE_LEFT;
          
		break;*/
	
	
	case PREDATOR:
		TextureManager::Instance()->loadSpriteSheet(
			"../Assets/sprites/predator.txt",
			"../Assets/sprites/predator.png",
			"predator"
		);
		setSpriteSheet(TextureManager::Instance()->getSpriteSheet("predator"));

		setWidth(128);
		setHeight(128);
		m_currentAnimationState = PREDATOR_IDLE_LEFT;
		break;
	
	
	default:
		break;
	}

	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(obsType);

	m_buildAnimations();
}

Obstacle::~Obstacle() = default;

void Obstacle::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	//draw the player according to animation state
	switch (getType())
	{
	case OBSTACLE1:
		TextureManager::Instance()->playAnimation("platsprites", getAnimation("spikes"),
			x, y, 0.12f, 0, 255, true);
		break;
	case OBSTACLE2:
		TextureManager::Instance()->playAnimation("Fireball_Sheet", getAnimation("fireball-active"),
			x, y, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case OBSTACLE3:
		TextureManager::Instance()->playAnimation("SpikeEnemies", getAnimation("Spike-Enemy"),
			x, y, 0.25f, 0, 255, true);
		break;
	case PLATFORM:
		TextureManager::Instance()->playAnimation("platform", getAnimation("stationary-platform"),
			x, y, 0.12f, 0, 255, true);
		break;
		
	case ENEMY:
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
    case PREDATOR:
		switch (m_currentAnimationState)
		{
		case PREDATOR_IDLE_RIGHT:
			TextureManager::Instance()->playAnimation("predator", getAnimation("predator-idle"),
				x, y, 0.12f, 0, 255, true);
			break;
		case PREDATOR_IDLE_LEFT:
		TextureManager::Instance()->playAnimation("predator", getAnimation("predator-idle"),
			x, y, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		 break;
		 }
		/*case ENEMY_RUN_RIGHT:
			TextureManager::Instance()->playAnimation("alien", getAnimation("enemy-run"),
				x, y, 0.25f, 0, 255, true);
			break;
		case ENEMY_RUN_LEFT:
			TextureManager::Instance()->playAnimation("alien", getAnimation("enemy-run"),
				x, y, 0.25f, 0, 255, true, SDL_FLIP_HORIZONTAL);
			break;*/
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void Obstacle::update()
{
	if (getType() == ENEMY)
	{
		// Update enemy
	}
	//else if (getType() == PREDATOR)
	//{
	//	// Update predator
	//}
}

void Obstacle::clean()
{
}

void Obstacle::setAnimationState(ObstacleAnimationState new_state)
{
	m_currentAnimationState = new_state;
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
	case OBSTACLE1: {

		Animation spikes = Animation();

		spikes.name = "spikes";
		spikes.frames.push_back(getSpriteSheet()->getFrame("hazard"));
		setAnimation(spikes);
	}
		break;
	case OBSTACLE2: {
		Animation fireball = Animation();

		fireball.name = "fireball-active";
		fireball.frames.push_back(getSpriteSheet()->getFrame("Fireball-1"));
		fireball.frames.push_back(getSpriteSheet()->getFrame("Fireball-2"));
		fireball.frames.push_back(getSpriteSheet()->getFrame("Fireball-3"));
		fireball.frames.push_back(getSpriteSheet()->getFrame("Fireball-4"));
		fireball.frames.push_back(getSpriteSheet()->getFrame("Fireball-5"));
		setAnimation(fireball);

		Animation fireballDie = Animation();
		fireballDie.name = "fireball-die";
		fireball.frames.push_back(getSpriteSheet()->getFrame("Fireball-die-1"));
		fireball.frames.push_back(getSpriteSheet()->getFrame("Fireball-die-2"));
		fireball.frames.push_back(getSpriteSheet()->getFrame("Fireball-die-3"));
		fireball.frames.push_back(getSpriteSheet()->getFrame("Fireball-die-4"));
		fireball.frames.push_back(getSpriteSheet()->getFrame("Fireball-die-5"));
		setAnimation(fireballDie);

	}
		break;
	case OBSTACLE3: {
		Animation spikeEnemy = Animation();

		spikeEnemy.name = "green-spike-enemy";
		spikeEnemy.frames.push_back(getSpriteSheet()->getFrame("Spike-Enemy-2"));
		setAnimation(spikeEnemy);
	}
		break;
	case PLATFORM: {
		Animation platform = Animation();

		platform.name = "stationary-platform";
		platform.frames.push_back(getSpriteSheet()->getFrame("plas"));

		setAnimation(platform);
	}
				 break;
	case ENEMY: {
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
			  break;

	case PREDATOR: {
		Animation idleAnimation = Animation();

		idleAnimation.name = "predator-idle";
		idleAnimation.frames.push_back(getSpriteSheet()->getFrame("predator-idle-0"));
		idleAnimation.frames.push_back(getSpriteSheet()->getFrame("predator-idle-1"));
		idleAnimation.frames.push_back(getSpriteSheet()->getFrame("predator-idle-2"));
		idleAnimation.frames.push_back(getSpriteSheet()->getFrame("predator-idle-3"));

		setAnimation(idleAnimation);

		Animation runAnimation = Animation();

		runAnimation.name = "predator-run";
		runAnimation.frames.push_back(getSpriteSheet()->getFrame("predator-run-0"));
		runAnimation.frames.push_back(getSpriteSheet()->getFrame("predator-run-1"));
		runAnimation.frames.push_back(getSpriteSheet()->getFrame("predator-run-2"));
		runAnimation.frames.push_back(getSpriteSheet()->getFrame("predator-run-3"));

		setAnimation(runAnimation);

		Animation deathAnimation = Animation();

		runAnimation.name = "predator-death";
		runAnimation.frames.push_back(getSpriteSheet()->getFrame("predator-death-0"));
		runAnimation.frames.push_back(getSpriteSheet()->getFrame("predator-death-1"));
		runAnimation.frames.push_back(getSpriteSheet()->getFrame("predator-death-2"));
		runAnimation.frames.push_back(getSpriteSheet()->getFrame("predator-death-3"));

		setAnimation(deathAnimation);
	}
			  break;
	default:
		break;
	}
}
