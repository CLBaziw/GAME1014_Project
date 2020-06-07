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
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
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
	}
	

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
	// Plane Sprite
	m_pPlaneSprite = new Plane();
	addChild(m_pPlaneSprite);

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	// Pause Button
	m_pPauseButton = new Button("../Assets/Menu Asset/Pause_BTN.png", "pauseButton", PAUSE_BUTTON);
	m_pPauseButton->getTransform()->position = glm::vec2(200.0f, 300.0f);
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
	m_pContinueButton = new Button("../Assets/Menu Asset/Play_BTN.png", "continueButton", CONTINUE_BUTTON);
	m_pContinueButton->getTransform()->position = glm::vec2(400.0f, 300.0f);
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
    m_pNextButton = new Button("../Assets/Menu Asset/Next_1.png", "nextButton", NEXT_BUTTON);
    m_pNextButton ->getTransform()->position = glm::vec2(600.0f, 500.0f);
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