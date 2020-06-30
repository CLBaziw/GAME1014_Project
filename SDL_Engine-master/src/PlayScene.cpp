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

	m_pPlayer->SetJumping(false);

	// Enemy Sprite - this will be removed later as enemies will not be spawned at scene start
	m_pEnemy = new Enemy();
	addChild(m_pEnemy);

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
	// Platform check
	if (COMA::AABBCheck(m_pPlayer, m_platform))
	{
		std::cout << "Player is under platform" << std::endl;
		if (m_pPlayer->getTransform()->position.y + m_pPlayer->getHeight() > m_platform->getTransform()->position.y) // Player's head <= top of platform
		{
			std::cout << "Player on platform" << std::endl;
			m_pPlayer->SetJumping(true);
			m_pPlayer->StopY();
			m_pPlayer->setPosition(m_pPlayer->getTransform()->position.x, m_platform->getTransform()->position.y - (m_pPlayer->getHeight() * 0.7));
		}
	}
	
	// Ground check
	if (m_pPlayer->getTransform()->position.y + 20 > m_ground->getTransform()->position.y - (m_pPlayer->getHeight() * 0.6))
	{
		std::cout << "Player on ground" << std::endl;
		m_pPlayer->SetJumping(true);
		m_pPlayer->StopY();
		m_pPlayer->setPosition(m_pPlayer->getTransform()->position.x, m_ground->getTransform()->position.y - (m_pPlayer->getHeight() * 0.7));
	}	
	
	// Player runs into enemy
	if (COMA::squaredRadiusCheck(m_pPlayer, m_pEnemy)) 
	{
		std::cout << "Player and enemy collide" << std::endl;
		// Kill player
		TheGame::Instance()->changeSceneState(END_SCENE);
	}

	for (int i = 0; i < m_pPlayerBulletVec.size(); i++)
	{
		if (COMA::squaredRadiusCheck(m_pEnemy, m_pPlayerBulletVec[i]))
		{
			std::cout << "Player killed enemy" << std::endl;
			
			/*delete m_pEnemy;
			m_pEnemy = nullptr;*/
			

			//delete m_pPlayerBulletVec[i];
			//m_pPlayerBulletVec[i] = nullptr;
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
