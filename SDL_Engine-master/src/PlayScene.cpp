#include "PlayScene.h"
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
	checkCollision();

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

void PlayScene::CheckBounds()
{
	// check left
	if (m_pPlayer->getTransform()->position.x > 1000 - m_pPlayer->getWidth() * 0.5)
	{
		m_pPlayer->setPosition(1000 - m_pPlayer->getWidth() * 0.5, m_pPlayer->getTransform()->position.y);
	}
	// check right
	if (m_pPlayer->getTransform()->position.x < 0 + m_pPlayer->getWidth() * 0.5)
	{
		m_pPlayer->setPosition(0 + m_pPlayer->getWidth() * 0.5, m_pPlayer->getTransform()->position.y);
	}
	// check up
	if (m_pPlayer->getTransform()->position.y < 0 + m_pPlayer->getHeight() * 0.5)
	{
		m_pPlayer->setPosition(m_pPlayer->getTransform()->position.x, 0 + m_pPlayer->getHeight() * 0.5);
	}
	// check down
	if (m_pPlayer->getTransform()->position.y > 600 - m_pPlayer->getHeight())
	{
		m_pPlayer->SetJumping(true);
		m_pPlayer->StopY();
		m_pPlayer->setPosition(m_pPlayer->getTransform()->position.x, 600 - m_pPlayer->getHeight());
	}
}

void PlayScene::checkCollision()
{
	for (int i = 0; i < m_pObstacles.size(); i++)
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