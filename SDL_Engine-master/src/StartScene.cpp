#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
{
	SoundManager::Instance().stopMusic();
}

void StartScene::draw()
{
	drawDisplayList();
}

void StartScene::update()
{
	updateDisplayList();
}

void StartScene::clean()
{
	std::cout << "Clean called on StartScene" << std::endl;

	delete m_pStartLabel;
	m_pStartLabel = nullptr;

	delete m_pShip;
	m_pShip = nullptr;

	delete m_pStartButton;
	m_pStartButton = nullptr;

	removeAllChildren();
}

void StartScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}
}

void StartScene::start()
{
	// Background
	m_pBackground = new Background("../Assets/backgrounds/startscene.png", "startscene-background", BACKGROUND, glm::vec2(12, 12), true);
	addChild(m_pBackground);

	const SDL_Color Metallicgold = { 212,175, 55, 0 };

	int windowWidth = TheGame::Instance()->getWindowWidth();
	int windowHeight = TheGame::Instance()->getWindowHeight();

	m_pStartLabel = new Label("M.E.T", "galaxy_1", 100, Metallicgold, glm::vec2(windowWidth / 2, windowHeight / 7));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);

	m_pNameLabel = new Label("ENTITY", "galaxy_1", 150, Metallicgold, glm::vec2(windowWidth / 2, windowHeight / 4));
	m_pNameLabel->setParent(this);
	addChild(m_pNameLabel);

	m_pShip = new Ship();
	m_pShip->getTransform()->position = glm::vec2(windowWidth /2 , windowHeight /2);
	addChild(m_pShip);

	// Start Button
	m_pStartButton = new Button();
	m_pStartButton->getTransform()->position = glm::vec2(windowWidth / 2, windowHeight / 2 + 150);

	m_pStartButton->addEventListener(CLICK, [&]()-> void
	{
		m_pStartButton->setActive(false);
		TheGame::Instance()->changeSceneState(LEVELSELECT_SCENE);
	});

	m_pStartButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pStartButton->setAlpha(128);
	});

	m_pStartButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pStartButton->setAlpha(255);
	});
	addChild(m_pStartButton);

	SoundManager::Instance().playMusic("startScreen");
}