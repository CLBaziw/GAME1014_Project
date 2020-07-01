#include "PlayScene.h"
#include "Obstacle.h"
#include "Game.h"
#include "EventManager.h"
#include "TextureManager.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();
}

void PlayScene::update()
{
	updateDisplayList();
	CheckBounds();
	MakeObstacles();
	checkCollision();
}

void PlayScene::clean()
{
	delete m_pPauseButton;
	m_pPauseButton = nullptr;

	delete m_pNextButton;
	m_pNextButton = nullptr;

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
}

void PlayScene::start()
{
	m_pBackground = new Background("../Assets/backgrounds/playscene.png", "playscene-background", BACKGROUND, glm::vec2 (0, 0), true);
	addChild(m_pBackground);

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	// Obstacle Creation 
	m_pObstacles.reserve(5);
	m_vec.reserve(10);
	m_numSpaces = 3;
	for (int i = 0; i < 9; i++)
	{
		m_vec.push_back(new Box(128 * i, 536));
	}
	m_pPlayer->SetJumping(false);

	// Enemy Sprite - this will be removed later as enemies will not be spawned at scene start
	m_pEnemy = new Enemy();
	addChild(m_pEnemy);

	/*m_pObstacle1 = new Obstacle(OBSTACLE1);
	addChild(m_pObstacle1);
	m_pObstacle2 = new Obstacle(OBSTACLE2);
	addChild(m_pObstacle2);
	m_pObstacle1 = new Obstacle(OBSTACLE3);
	addChild(m_pObstacle3);*/

	// CREATE OBSTACLE HERE - Like above ^
	// You want to make sure to randomize which obstacle will be created as we will have more than one option 
	// Enum options can be used like integers starting with 0 so you can select a type using the 0-2 or however many options you have
	m_platform = new Platform(380, 400);
	addChild(m_platform);

	//Ground
	m_ground = new ground(0, 587);
	addChild(m_ground);

	std::cout << "The height of the ground: " << m_ground->getHeight() << std::endl;


	// Bullets
	m_pPlayerBulletVec.reserve(10);

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


// Next Button
    m_pNextButton = new Button("../Assets/Menu Asset/Next_1_small.png", "nextButton", NEXT_BUTTON);
    m_pNextButton ->getTransform()->position = glm::vec2(830, 80.0f);
    m_pNextButton->addEventListener(CLICK, [&]()-> void
    {
    	m_pNextButton->setActive(false);
    	TheGame::Instance()->changeSceneState(END_SCENE);
    });
    
    m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
    {
    	m_pNextButton->setAlpha(128);
    });
    
    m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
    {
    	m_pNextButton->setAlpha(255);
    });
    
    addChild(m_pNextButton);
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
		std::cout << "Player on ground" << std::endl;
		m_pPlayer->SetJumping(true);
		m_pPlayer->StopY();
		m_pPlayer->setPosition(playerX, groundY - halfPlayerHeight - 15);
	}

	// This will change into m_pObstacles and be inside a loop for later.
	int platformX = m_platform->getTransform()->position.x;
	int platformY = m_platform->getTransform()->position.y;
	int halfPlatformWidth = m_platform->getWidth() * 0.5;
	int halfPlatformHeight = m_platform->getHeight() * 0.5;

	// Platform check
	if (playerY < platformY - halfPlatformHeight)
	{
		std::cout << "Player above platform" << std::endl;
		if ((playerX + halfPlayerWidth < platformX + halfPlatformWidth || playerX - halfPlayerWidth < platformX + halfPlatformWidth)
			&& (playerX + halfPlayerWidth > platformX - halfPlatformWidth || playerX - halfPlayerWidth > platformX - halfPlatformWidth))
		{
			std::cout << "Player on platform" << std::endl;
			m_pPlayer->SetJumping(true);
			m_pPlayer->StopY();
			m_pPlayer->setPosition(playerX, platformY - halfPlayerHeight - 30);
		}
		else if (playerX > platformX + halfPlatformWidth || playerX < platformX - halfPlatformWidth)
		{
			std::cout << "Player should fall" << std::endl;
			m_pPlayer->SetJumping(false);
		}
	}

	//for (int i = 0; i < m_pObstacles.size(); i++)
	
	// Player runs into enemy
	if (COMA::squaredRadiusCheck(m_pPlayer, m_pEnemy)) 
	{
		if (COMA::circleAABBCheck(m_pPlayer, m_pObstacles[i]))
		{
			std::cout << "Player colliding with obstacle" << std::endl;
			if (m_pObstacles[i]->GetSafe())
			{
				if (m_pPlayer->getTransform()->position.x + m_pPlayer->getWidth() - m_pPlayer->getRigidBody()->velocity.x <= m_pObstacles[i]->getTransform()->position.x)
				{ // Collision from left of obstacle.
					m_pPlayer->StopX(); // Stop the player from moving horizontally.
					m_pPlayer->setPosition(m_pObstacles[i]->getTransform()->position.x - m_pPlayer->getWidth(), m_pPlayer->getTransform()->position.y);
				}
				else if (m_pPlayer->getTransform()->position.x - m_pPlayer->getRigidBody()->velocity.x >= m_pObstacles[i]->getTransform()->position.x + m_pObstacles[i]->getWidth())
				{ // Collision from right of obstacle.
					m_pPlayer->StopX();
					m_pPlayer->setPosition(m_pObstacles[i]->getTransform()->position.x + m_pObstacles[i]->getWidth(), m_pPlayer->getTransform()->position.y);
				}
				else if (m_pPlayer->getTransform()->position.y + m_pPlayer->getHeight() - m_pPlayer->getRigidBody()->velocity.y <= m_pObstacles[i]->getTransform()->position.y)
				{ // Collision from top side of obstacle.
					m_pPlayer->SetJumping(true);
					m_pPlayer->StopY();
					m_pPlayer->setPosition(m_pPlayer->getTransform()->position.x, m_pObstacles[i]->getTransform()->position.y - m_pPlayer->getHeight() - 1);
				}
				else if (m_pPlayer->getTransform()->position.y - m_pPlayer->getRigidBody()->velocity.y >= m_pObstacles[i]->getTransform()->position.y + m_pObstacles[i]->getHeight())
				{ // Collision from bottom side of obstacle.
					m_pPlayer->StopY();
					m_pPlayer->setPosition(m_pPlayer->getTransform()->position.x, m_pObstacles[i]->getTransform()->position.y + m_pObstacles[i]->getHeight());
				}
			}
			else
			{
				std::cout << "Player and enemy collide" << std::endl;
				// Kill player
				TheGame::Instance()->changeSceneState(END_SCENE);
			}
		}

		// Check for bullet with enemy
		for (int j = 0; j < m_pPlayerBulletVec.size(); j++)
		{
			if (m_pObstacles[i]->getType() == ENEMY)
			{
				if (COMA::squaredRadiusCheck(m_pObstacles[i], m_pPlayerBulletVec[j]))
				{
					std::cout << "Player killed enemy" << std::endl;

					/*delete m_pEnemy;
					m_pEnemy = nullptr;*/
					//delete m_pPlayerBulletVec[i];
					//m_pPlayerBulletVec[i] = nullptr;
				}
			}
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
		x = m_pPlayer->getTransform()->position.x + 40;
	}
	else
	{
		bState = BULLET_MOVE_LEFT;
		x = m_pPlayer->getTransform()->position.x - 40;
	}

	m_pPlayerBulletVec.push_back(new Bullet(x, y, true, bState));
	addChild(m_pPlayerBulletVec[m_pPlayerBulletVec.size() - 1]);
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
			m_vec.push_back(new Box(128 * (m_vec.size() + 1), 536, true));

			m_pObstacles.push_back(m_vec.back()->GetSprite());
			addChild(m_pObstacles[m_pObstacles.size() - 1]);

			if (m_pObstacles.size() > 4)
			{
				m_pObstacles[0] = nullptr;
				m_pObstacles.erase(m_pObstacles.begin());

				// Remove child
			}

			m_numSpaces = 0;
		}
		else // Create another empty space
		{
			m_vec.push_back(new Box(128 * m_vec.size(), 384, false));
		}
	}

	// Scroll the boxes.
	for (int col = 0; col < m_vec.size(); col++)
	{
		m_vec[col]->Update();
	}
}