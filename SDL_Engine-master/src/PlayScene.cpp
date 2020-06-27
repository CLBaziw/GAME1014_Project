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



	// Enemy Sprite - this will be removed later as enemies will not be spawned at scene start
	m_pEnemy = new Enemy();
	addChild(m_pEnemy);

	// CREATE OBSTACLE HERE - Like above ^
	// You want to make sure to randomize which obstacle will be created as we will have more than one option 
	// Enum options can be used like integers starting with 0 so you can select a type using the 0-2 or however many options you have
	m_platform = new Platform(380, 450);
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

void PlayScene::CheckBounds()
{
	// check right
	if (m_pPlayer->getTransform()->position.x > m_ground->getWidth() - m_pPlayer->getWidth() * 0.5)
	{
		m_pPlayer->setPosition(m_ground->getWidth() - m_pPlayer->getWidth() * 0.5, m_pPlayer->getTransform()->position.y);
	}
	// check left
	if (m_pPlayer->getTransform()->position.x < 0 + m_pPlayer->getWidth() * 0.5)
	{
		m_pPlayer->setPosition(0 + m_pPlayer->getWidth() * 0.5, m_pPlayer->getTransform()->position.y);
	}
	// check up
	if (m_pPlayer->getTransform()->position.y < 0 + m_pPlayer->getHeight() * 0.5)
	{
		m_pPlayer->setPosition(m_pPlayer->getTransform()->position.x, 0 + m_pPlayer->getHeight() * 0.5);
	}
	//// check down
	if (m_pPlayer->getTransform()->position.y > m_ground->getTransform()->position.y - (m_pPlayer->getHeight() * 0.6))
	{
		m_pPlayer->SetJumping(true);
		m_pPlayer->StopY();
		m_pPlayer->setPosition(m_pPlayer->getTransform()->position.x, m_ground->getTransform()->position.y - (m_pPlayer->getHeight()*0.6));
	}
}

void PlayScene::checkCollision()
{
	 //PLATFORM CHECKS
	//if (COMA::AABBCheck(m_pPlayer, m_platform))
	//{
	//	if (c_pPlayer->x + c_pPlayer->w - m_pPlayer->GetVelX() <= c_platform->x)
	//	{ // Collision from left of obstacle.
	//		m_pPlayer->StopX(); // Stop the player from moving horizontally.
	//		m_pPlayer->SetX(c_platform->x - c_pPlayer->w);
	//	}
	//	else if (c_pPlayer->x - (float)m_pPlayer->GetVelX() >= c_platform->x + c_platform->w)
	//	{ // Collision from right of obstacle.
	//		m_pPlayer->StopX();
	//		m_pPlayer->SetX(c_platform->x + c_platform->w);
	//	}
	//	else if (c_pPlayer->y + c_pPlayer->h - (float)m_pPlayer->GetVelY() <= c_platform->y)
	//	{ // Collision from top side of obstacle.
	//		m_pPlayer->SetJumping(true);
	//		m_pPlayer->StopY();
	//		m_pPlayer->SetY(c_platform->y - c_pPlayer->h - 1);
	//	}
	//	else if (c_pPlayer->y - (float)m_pPlayer->GetVelY() >= c_platform->y + c_platform->h)
	//	{ // Collision from bottom side of obstacle.
	//		m_pPlayer->StopY();
	//		m_pPlayer->SetY(c_platform->y + c_platform->h);
	//	}
	//}	
	if (COMA::AABBCheck(m_pPlayer, m_platform))
	{
		std::cout << "Colliding" << std::endl;
		if (m_pPlayer->getDst().x + m_pPlayer->getDst().w - m_pPlayer->getRigidBody()->velocity.x <= m_platform->getDst().x)
		{ // Collision from left of obstacle.
			m_pPlayer->StopX(); // Stop the player from moving horizontally.
			m_pPlayer->SetX(m_platform->getDst().x - m_pPlayer->getDst().w);
		}
		else if (m_pPlayer->getDst().x - (float)m_pPlayer->getRigidBody()->velocity.x >= m_platform->getDst().x + m_platform->getDst().w)
		{ // Collision from right of obstacle.
			m_pPlayer->StopX();
			m_pPlayer->SetX(m_platform->getDst().x + m_platform->getDst().w);
		}
		else if (m_pPlayer->getDst().y + m_pPlayer->getDst().h - (float)m_pPlayer->getRigidBody()->velocity.y <= m_platform->getDst().y)
		{ // Collision from top side of obstacle.
			m_pPlayer->SetJumping(true);
			m_pPlayer->StopY();
			m_pPlayer->SetY(m_platform->getDst().y - m_pPlayer->getDst().h - 1);
		}
		else if (m_pPlayer->getDst().y - (float)m_pPlayer->getRigidBody()->velocity.y >= m_platform->getDst().y + m_platform->getDst().h)
		{ // Collision from bottom side of obstacle.
			m_pPlayer->StopY();
			m_pPlayer->SetY(m_platform->getDst().y + m_platform->getDst().h);
		}
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
			

			/*delete m_pPlayerBulletVec[i];
			m_pPlayerBulletVec[i] = nullptr;*/
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
