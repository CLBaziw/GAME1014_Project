#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

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
	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	// Enemy Sprite - this will be removed later as enemies will not be spawned at scene start
	m_pEnemy = new Enemy();
	addChild(m_pEnemy);

	// Pause Button
	m_pPauseButton = new Button("../Assets/Menu Asset/Pause_BTN_small.png", "pauseButton", PAUSE_BUTTON);
	m_pPauseButton->getTransform()->position = glm::vec2(60.0f, 50.0f);
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
	m_pContinueButton->getTransform()->position = glm::vec2(150.0f, 50.0f);
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
    m_pNextButton ->getTransform()->position = glm::vec2(640, 50.0f);
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
	if (m_pPlayer->getTransform()->position.x > 800 - m_pPlayer->getWidth() * 0.5)
	{
		m_pPlayer->setPosition(800 - m_pPlayer->getWidth() * 0.5, m_pPlayer->getTransform()->position.y);
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
	// PLATFORM CHECKS
	//if (COMA::AABBCheck(m_pPlayer, m_pPlaneSprite))
	//{
	//	if (m_pPlayer->getTransform()->position.x + m_pPlayer->getWidth() - m_pPlayer->GetVelX() <= m_pPlaneSprite->getTransform()->position.x)
	//	{ // Collision from left of obstacle.
	//		m_pPlayer->StopX(); // Stop the player from moving horizontally.
	//		m_pPlayer->setPosition(m_pPlaneSprite->getTransform()->position.x - m_pPlayer->getWidth(), m_pPlayer->getTransform()->position.y);
	//	}
	//	else if (m_pPlayer->getTransform()->position.x - (float)m_pPlayer->GetVelX() >= m_pPlaneSprite->getTransform()->position.x + m_pPlaneSprite->getWidth())
	//	{ // Collision from right of obstacle.
	//		m_pPlayer->StopX();
	//		m_pPlayer->setPosition(m_pPlaneSprite->getTransform()->position.x + m_pPlaneSprite->getWidth(), m_pPlayer->getTransform()->position.y);
	//	}
	//	else if (m_pPlayer->getTransform()->position.y + m_pPlayer->getHeight() - (float)m_pPlayer->GetVelY() <= m_pPlaneSprite->getTransform()->position.y)
	//	{ // Collision from top side of obstacle.
	//		m_pPlayer->SetJumping(true);
	//		m_pPlayer->StopY();
	//		m_pPlayer->setPosition(m_pPlayer->getTransform()->position.x, m_pPlaneSprite->getTransform()->position.y - m_pPlayer->getHeight() - 1);
	//	}
	//	else if (m_pPlayer->getTransform()->position.y - (float)m_pPlayer->GetVelY() >= m_pPlaneSprite->getTransform()->position.y + m_pPlaneSprite->getHeight())
	//	{ // Collision from bottom side of obstacle.
	//		m_pPlayer->StopY();
	//		m_pPlayer->setPosition(m_pPlayer->getTransform()->position.x, m_pPlaneSprite->getTransform()->position.y + m_pPlaneSprite->getHeight());
	//	}
	//}

	if (COMA::squaredRadiusCheck(m_pPlayer, m_pEnemy)) 
	{
		std::cout << "Player and enemy collide" << std::endl;
		// Kill player
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}