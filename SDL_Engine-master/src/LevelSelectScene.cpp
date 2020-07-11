#include "LevelSelectScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

LevelSelectScene::LevelSelectScene()
{
	LevelSelectScene::start();
}

LevelSelectScene::~LevelSelectScene()
{
	SoundManager::Instance().stopMusic();
}

void LevelSelectScene::draw()
{
	drawDisplayList();
}

void LevelSelectScene::update()
{
	updateDisplayList();
}

void LevelSelectScene::clean()
{
	std::cout << "Clean called on LevelSelectScene" << std::endl;

	delete m_pLevelOneButton;
	m_pLevelOneButton = nullptr;

	delete m_pLevelTwoButton;
	m_pLevelTwoButton = nullptr;

	removeAllChildren();
}

void LevelSelectScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	}
}

void LevelSelectScene::start()
{
	// Background
	m_pBackground = new Background("../Assets/backgrounds/startscene.png", "startscene-background", BACKGROUND, glm::vec2(12, 12), true);
	addChild(m_pBackground);

	const SDL_Color LightBlue = { 0,255, 255, 0 };

	m_pLevelSelectLabel = new Label("Please select level", "galaxy_1", 60, LightBlue, glm::vec2(576.0f, 120.0f));
	m_pLevelSelectLabel->setParent(this);
	addChild(m_pLevelSelectLabel);


	// Level 1 Button
	m_pLevelOneButton = new Button("../Assets/Menu Asset/level-1.png", "LevelOneButton", LEVEL1_BUTTON);
	m_pLevelOneButton->getTransform()->position = glm::vec2(576.0f, 350.0f);
	m_pLevelOneButton->addEventListener(CLICK, [&]()-> void
	{
		m_pLevelOneButton->setActive(false);
		TheGame::Instance()->setLevel(0);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});

	m_pLevelOneButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pLevelOneButton->setAlpha(128);
	});

	m_pLevelOneButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pLevelOneButton->setAlpha(255);
	});

	addChild(m_pLevelOneButton);


	// Level 2 Button
	m_pLevelTwoButton = new Button("../Assets/Menu Asset/level-2.png", "LevelTwoButton", LEVEL2_BUTTON);
	m_pLevelTwoButton->getTransform()->position = glm::vec2(576.0f, 540.0f);
	m_pLevelTwoButton->addEventListener(CLICK, [&]()-> void
	{
		m_pLevelTwoButton->setActive(false);
		TheGame::Instance()->setLevel(1);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});

	m_pLevelTwoButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pLevelTwoButton->setAlpha(128);
	});

	m_pLevelTwoButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pLevelTwoButton->setAlpha(255);
	});

	addChild(m_pLevelTwoButton);

	SoundManager::Instance().playMusic("startScreen");
}


