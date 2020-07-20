#include "PlayScene.h"
#include <algorithm>
#include "Obstacle.h"
#include "Game.h"
#include "EventManager.h"
#include "TextureManager.h"

#define ENEMYSIGHT 280

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
{
	SoundManager::Instance().stopMusic();
}

void PlayScene::draw()
{
	drawDisplayList();
	m_objPool->DrawActiveSprites();
}

void PlayScene::update()
{
	m_objPool->UpdateActiveSprites();
	updateDisplayList();
	MakeObstacles();
	checkCollision();
}

void PlayScene::clean()
{
	// clean GameObject
	delete m_pBackground;
	m_pBackground = nullptr;

	delete m_pScoreBoard;
	m_pScoreBoard = nullptr;

	delete m_pPlayer;
	m_pPlayer = nullptr;

	delete m_ground;
	m_ground = nullptr;

	delete m_objPool;
	m_objPool = nullptr;

	for (int i = 0; i < m_vec.size(); i++)
	{
		delete m_vec[i];
		m_vec[i] = nullptr;
	}	
	
	for (int i = 0; i < m_pObstacles.size(); i++)
	{
		delete m_pObstacles[i];
		m_pObstacles[i] = nullptr;
	}	
	
	for (int i = 0; i < m_pPlayerBulletVec.size(); i++)
	{
		delete m_pPlayerBulletVec[i];
		m_pPlayerBulletVec[i] = nullptr;
	}	
	
	for (int i = 0; i < m_pEnemyBulletVec.size(); i++)
	{
		delete m_pEnemyBulletVec[i];
		m_pEnemyBulletVec[i] = nullptr;
	}

	// clean button
	delete m_pPauseButton;
	m_pPauseButton = nullptr;

	delete m_pContinueButton;
	m_pContinueButton = nullptr;

	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}

	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			moveX = -1;
			m_pPlayer->getTransform()->position.x += (moveX * moveSpeed);
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			moveX = 1;
			m_pPlayer->getTransform()->position.x += (moveX * moveSpeed);
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}

		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_SPACE) && m_pPlayer->isJumping())
		{
			m_pPlayer->SetAccelY(-JUMPFORCE);
			m_pPlayer->SetJumping(false);
			SoundManager::Instance().playSound("jump");
		}

		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_LSHIFT) && !m_pPlayer->isShooting())
		{
			m_pPlayer->SetShooting(true);
			PlayerShoot(); 
		}
		else if (EventManager::Instance().isKeyUp(SDL_SCANCODE_LSHIFT) && m_pPlayer->isShooting())
		{
			m_pPlayer->SetShooting(false);
		}
	}

	m_pPlayer->update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
	EnemyShoot();
}

void PlayScene::start()
{
	//Differentiate between levels
	if (TheGame::Instance()->getLevel() == 0)
	{
		m_pBackground = new Background("../Assets/backgrounds/playscene.png", "playscene-background", BACKGROUND, glm::vec2(0, 0), true);
	}
	else
	{
		m_pBackground = new Background("../Assets/backgrounds/playscene2.png", "playscene-background", BACKGROUND, glm::vec2(0, 0), true);
	}
	
	// Object Pool
	m_objPool = new ObjectPool();

	// Background 
	addChild(m_pBackground);

	//Score Board
	const SDL_Color yellow = { 255, 255, 0, 255 };
	m_pScoreBoard = new ScoreBoard("Score:", "Playbill", 60, yellow, glm::vec2(1000.0f, 80.0f));;
	m_pScoreBoard->setParent(this);
	addChild(m_pScoreBoard);

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	// Obstacle Creation 
	m_pObstacles.reserve(4);
	m_vec.reserve(10);
	m_numSpaces = 3;
	for (int i = 0; i < 10; i++)
	{
		m_vec.push_back(new Box(128 * i, 520));
	}
	m_pPlayer->SetJumping(false);

	//Ground
	m_ground = new ground(0, 588);
	addChild(m_ground);

	// Bullets
	m_pPlayerBulletVec.reserve(10);
	/*m_pEnemyBulletVec.reserve(30);*/
}

void PlayScene::checkCollision()
{
	int playerX = m_pPlayer->getTransform()->position.x;
	int playerY = m_pPlayer->getTransform()->position.y;
	int halfPlayerWidth = m_pPlayer->getWidth() * 0.5;
	int halfPlayerHeight = m_pPlayer->getHeight() * 0.5;
	int groundY = m_ground->getTransform()->position.y;

	// Ground check
	if (playerY > groundY - halfPlayerHeight - 20)
	{
		m_pPlayer->SetJumping(true);
		m_pPlayer->StopY();
		m_pPlayer->setPosition(playerX, groundY - halfPlayerHeight - 15);
	}

	for (int i = 0; i < m_pObstacles.size(); i++)
	{
		switch (m_pObstacles[i]->getType())
		{
		case PLATFORM:
		{
			int platformX = m_pObstacles[i]->getTransform()->position.x;
			int platformY = m_pObstacles[i]->getTransform()->position.y;
			int halfPlatformWidth = m_pObstacles[i]->getWidth() * 0.5;
			int halfPlatformHeight = m_pObstacles[i]->getHeight() * 0.5;

			if (playerY < platformY - halfPlatformHeight)
			{
				if ((playerX + halfPlayerWidth < platformX + halfPlatformWidth || playerX - halfPlayerWidth < platformX + halfPlatformWidth)
					&& (playerX + halfPlayerWidth > platformX - halfPlatformWidth || playerX - halfPlayerWidth > platformX - halfPlatformWidth))
				{
					m_pPlayer->SetJumping(true);
					m_pPlayer->StopY();
					m_pPlayer->setPosition(playerX, platformY - halfPlayerHeight - 30);
				}
				else if (playerX > platformX + halfPlatformWidth || playerX < platformX - halfPlatformWidth)
				{
					m_pPlayer->SetJumping(false);
				}
			}
			break;
		}
		case PREDATOR:
		{
			if (COMA::squaredRadiusCheck(m_pPlayer, m_pObstacles[i])) // Player and enemy collide
			{
				std::cout << "Player and enemy collide" << std::endl;
				//PlayerDeath();
			}

			// Check for bullet with enemy
			for (int j = 0; j < m_pPlayerBulletVec.size(); j++)
			{
				if (COMA::squaredRadiusCheck(m_pObstacles[i], m_pPlayerBulletVec[j]))
				{
					std::cout << "Player killed enemy" << std::endl;

					m_pObstacles[i]->setActive(false);
					m_pObstacles[i] = nullptr;
					m_pObstacles.erase(m_pObstacles.begin() + i);
				}
			}
			break;
		}
		case ENEMY:
		{
			if (COMA::squaredRadiusCheck(m_pPlayer, m_pObstacles[i])) // Player and enemy collide
			{
				std::cout << "Player and enemy collide" << std::endl;
				//PlayerDeath();
			}

			// Check for bullet with enemy
			for (int j = 0; j < m_pPlayerBulletVec.size(); j++)
			{
				if (COMA::squaredRadiusCheck(m_pObstacles[i], m_pPlayerBulletVec[j]))
				{
					std::cout << "Player killed enemy" << std::endl;

					m_pObstacles[i]->setActive(false);
					m_pObstacles[i] = nullptr;
					m_pObstacles.erase(m_pObstacles.begin() + i);
				}
			}
			break;
		}
		case OBSTACLE1:
		case OBSTACLE2:
		case OBSTACLE3:
		//{
		//	if (COMA::squaredRadiusCheck(m_pPlayer, m_pObstacles[i]))
		//	{
		//		std::cout << "Player died to obstacle" << std::endl;
		//		PlayerDeath();
		//	}
		//	break;
		//}
		case E_BULLET:
			for (int i = 0; i < m_pEnemyBulletVec.size(); i++)
			{
				if (COMA::squaredRadiusCheck(m_pPlayer, m_pEnemyBulletVec[i]))
				{
					std::cout << "Enemy shot player" << std::endl;
					PlayerDeath();
				}
			}
			break;
		}
	}
}

void PlayScene::PlayerShoot()
{
	float x;
	float y = m_pPlayer->getTransform()->position.y;

	BulletAnimationState bState;

	if (m_playerFacingRight)
	{
		bState = BULLET_MOVE_RIGHT;
		x = m_pPlayer->getTransform()->position.x + 20;
	}
	else
	{
		bState = BULLET_MOVE_LEFT;
		x = m_pPlayer->getTransform()->position.x - 10;
	}

	m_pPlayerBulletVec.push_back(new Bullet(x, y, /*true*/PLAYER_BULLET, bState));
	addChild(m_pPlayerBulletVec[m_pPlayerBulletVec.size() - 1]);

	SoundManager::Instance().playSound("shot");
}

void PlayScene::MakeObstacles()
{
	// Check for out of bounds.
	if ((m_vec[0])->GetX() <= -128) // Fully off-screen.
	{
		m_numSpaces++;
		// Remove front element.
		delete m_vec[0];
		m_vec[0] = nullptr;
		m_vec.erase(m_vec.begin());

		// Add new box.
		if (m_numSpaces >= 3) // Add new sprite if there has been enough spaces.
		{
			m_vec.push_back(new Box(128 * (m_vec.size() + 1), 536));

			m_pObstacles.push_back(m_vec.back()->GetRandomObstacle(m_objPool, m_vec.back()->GetX(), 536));

			if (m_pObstacles.size() > 4)
			{
				std::cout << "Remove obstacle" << std::endl;
				m_pObstacles[0]->setActive(false);
				m_pObstacles[0] = nullptr;
				m_pObstacles.erase(m_pObstacles.begin());
			}

			m_numSpaces = 0;
		}
		else // Create another empty space
		{
			m_vec.push_back(new Box(128 * m_vec.size(), 536));
		}
	}

	// Scroll the boxes.
	for (int col = 0; col < m_vec.size(); col++)
	{
		m_vec[col]->Update();
	}
}

void PlayScene::EnemyShoot()
{
	for (int i = 0; i < m_pObstacles.size(); i++)
	{
		if (m_pObstacles[i]->getType() == PREDATOR)
		{
			Obstacle* enemy = m_pObstacles[i];
			float enemyX = enemy->getTransform()->position.x;
			float enemyY = enemy->getTransform()->position.y;
			float playerX = m_pPlayer->getTransform()->position.x;
			float playerY = m_pPlayer->getTransform()->position.y;
			if (playerX > enemyX - ENEMYSIGHT - m_pPlayer->getWidth() && playerX < enemyX && playerY >= enemyY)
			{
				enemyX = m_pObstacles[i]->getTransform()->position.x - 85.0f; 
				enemyY = m_pObstacles[i]->getTransform()->position.y - 10.0f;
			
				enemy->setAnimationState(PREDATOR_IDLE_LEFT);	
	
				
				if (m_bulletTimer++ == m_timerMax)
				{
					m_pEnemyBulletVec.push_back(new Bullet(enemyX, enemyY, /*false*/ ENEMY_BULLET, BULLET_MOVE_LEFT));
					m_pEnemyBulletVec[m_pEnemyBulletVec.size() - 1]->setType(E_BULLET);
					addChild(m_pEnemyBulletVec[m_pEnemyBulletVec.size() - 1]);
					m_bulletTimer = 0;
				}
			}
			else if (playerX < enemyX + ENEMYSIGHT + m_pPlayer->getWidth() && playerX > enemyX && playerX >= enemyY)
			{
				enemyX = m_pObstacles[i]->getTransform()->position.x + 85.0f;
				enemyY = m_pObstacles[i]->getTransform()->position.y - 10.0f;
				
				enemy->setAnimationState(PREDATOR_IDLE_RIGHT);
			
				if (m_bulletTimer++ == m_timerMax)
				{
					m_pEnemyBulletVec.push_back(new Bullet(enemyX, enemyY, /*false*/ ENEMY_BULLET, BULLET_MOVE_RIGHT));
					m_pEnemyBulletVec[m_pEnemyBulletVec.size() - 1]->setType(E_BULLET);
					addChild(m_pEnemyBulletVec[m_pEnemyBulletVec.size() - 1]);
					m_bulletTimer = 0;
				}
			}
		}
		else if (m_pObstacles[i]->getType() == ENEMY)
		{
			Obstacle* enemy = m_pObstacles[i];
			float enemyX = enemy->getTransform()->position.x;
			float enemyY = enemy->getTransform()->position.y;
			float playerX = m_pPlayer->getTransform()->position.x;
			float playerY = m_pPlayer->getTransform()->position.y;
			if (playerX > enemyX - ENEMYSIGHT - m_pPlayer->getWidth() && playerX < enemyX && playerY >= enemyY)
			{
				enemyX = m_pObstacles[i]->getTransform()->position.x - 85.0f; 
				enemyY = m_pObstacles[i]->getTransform()->position.y - 10.0f;
				enemy->setAnimationState(ENEMY_IDLE_LEFT);
				if (m_bulletTimer++ == m_timerMax)
				{
					m_pEnemyBulletVec.push_back(new Bullet(enemyX, enemyY, /*false*/ENEMY_BULLET, BULLET_MOVE_LEFT));
					m_pEnemyBulletVec[m_pEnemyBulletVec.size() - 1]->setType(E_BULLET);
					addChild(m_pEnemyBulletVec[m_pEnemyBulletVec.size() - 1]);
					m_bulletTimer = 0;
				}
			}
			else if (playerX < enemyX + ENEMYSIGHT + m_pPlayer->getWidth() && playerX > enemyX && playerX >= enemyY)
			{
				enemyX = m_pObstacles[i]->getTransform()->position.x + 85.0f;
				enemyY = m_pObstacles[i]->getTransform()->position.y - 10.0f;
				enemy->setAnimationState(ENEMY_IDLE_RIGHT);
				if (m_bulletTimer++ == m_timerMax)
				{
					m_pEnemyBulletVec.push_back(new Bullet(enemyX, enemyY, /*false*/ENEMY_BULLET, BULLET_MOVE_RIGHT));
					m_pEnemyBulletVec[m_pEnemyBulletVec.size() - 1]->setType(E_BULLET);
					addChild(m_pEnemyBulletVec[m_pEnemyBulletVec.size() - 1]);
					m_bulletTimer = 0;
				}
			}
		}
	}
}

void PlayScene::PlayerDeath()
{
	TheGame::Instance()->changeSceneState(END_SCENE);
}
