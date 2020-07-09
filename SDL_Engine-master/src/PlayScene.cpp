#include "PlayScene.h"
#include "Obstacle.h"
#include "Game.h"
#include "EventManager.h"
#include "TextureManager.h"

#define ENEMYSIGHT 250

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
	delete m_pPauseButton;
	m_pPauseButton = nullptr;

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

	// Object Pool
	m_objPool = new ObjectPool();

	// Background
	m_pBackground = new Background("../Assets/backgrounds/playscene.png", "playscene-background", BACKGROUND, glm::vec2 (0, 0), true);
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
	m_ground = new ground(0, 587);
	addChild(m_ground);

	// Bullets
	m_pPlayerBulletVec.reserve(10);
	/*m_pEnemyBulletVec.reserve(30);*/

	// Pause Button
	m_pPauseButton = new Button("../Assets/Menu Asset/Pause_BTN_small.png", "pauseButton", PAUSE_BUTTON);
	m_pPauseButton->getTransform()->position = glm::vec2(80.0f, 80.0f);
	m_pPauseButton->addEventListener(CLICK, [&]()-> void
	{
		m_pPauseButton->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});

	m_pPauseButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pPauseButton->setAlpha(128);
	});

	m_pPauseButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pPauseButton->setAlpha(255);
	});
	addChild(m_pPauseButton);

	// Continue Button
	m_pContinueButton = new Button("../Assets/Menu Asset/Play_BTN_small.png", "continueButton", CONTINUE_BUTTON);
	m_pContinueButton->getTransform()->position = glm::vec2(170.0f, 80.0f);
	m_pContinueButton->addEventListener(CLICK, [&]()-> void
	{
		m_pContinueButton->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});

	m_pContinueButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pContinueButton->setAlpha(128);
	});

	m_pContinueButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pContinueButton->setAlpha(255);
	});

	addChild(m_pContinueButton);
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
		if (m_pObstacles[i]->getType() == PLATFORM) // Platform check
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
		}
		else if (m_pObstacles[i]->getType() == ENEMY) // Enemy check
		{
			if (COMA::squaredRadiusCheck(m_pPlayer, m_pObstacles[i])) // Player and enemy collide
			{
				std::cout << "Player and enemy collide" << std::endl;
				// Kill player
				TheGame::Instance()->changeSceneState(END_SCENE);
			}

			// Check for bullet with enemy
			/*for (int j = 0; j < m_pPlayerBulletVec.size(); j++)
			{
				if (COMA::squaredRadiusCheck(m_pObstacles[i], m_pPlayerBulletVec[j]))
				{
					std::cout << "Player killed enemy" << std::endl;

					m_pObstacles[i]->setActive(false);
					m_pObstacles[i] = nullptr;
					m_pObstacles.erase(m_pObstacles.begin() + i);
				}
			}*/
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

	m_pPlayerBulletVec.push_back(new Bullet(x, y, true, bState));
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
		if (m_pObstacles[i]->getType() == ENEMY)
		{
			Obstacle* enemy = m_pObstacles[i];
			float enemyX = enemy->getTransform()->position.x;
			float enemyY = enemy->getTransform()->position.y;
			float playerX = m_pPlayer->getTransform()->position.x;
			float playerY = m_pPlayer->getTransform()->position.y;
			if (playerX < enemyX && playerY >= enemyY)
			{
				enemyX = m_pObstacles[i]->getTransform()->position.x - 48;
				enemy->setAnimationState(ENEMY_IDLE_LEFT);
				if (m_bulletTimer++ == m_timerMax)
				{
					m_pEnemyBulletVec.push_back(new Bullet(enemyX, enemyY, false, BULLET_MOVE_LEFT));
					addChild(m_pEnemyBulletVec[m_pEnemyBulletVec.size() - 1]);
					m_bulletTimer = 0;
				}
			}
			if (playerX + ENEMYSIGHT > enemyX && playerX >= enemyY)
			{
				enemyX = enemyX + 48;
				enemy->setAnimationState(ENEMY_IDLE_RIGHT);
				if (m_bulletTimer++ == m_timerMax)
				{
					m_pEnemyBulletVec.push_back(new Bullet(enemyX, enemyY, false, BULLET_MOVE_RIGHT));
					addChild(m_pEnemyBulletVec[m_pEnemyBulletVec.size() - 1]);
					m_bulletTimer = 0;
				}
			}
		}
	}
}
