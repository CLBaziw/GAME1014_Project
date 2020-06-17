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
= default;

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

	delete m_pInstructionsLabel;
	m_pInstructionsLabel = nullptr;

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

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	}
}

void StartScene::start()
{
	// Background
	m_pBackground = new Background("../Assets/backgrounds/playscene.png", "startscene-background", BACKGROUND, glm::vec2(0, 0), true);
	addChild(m_pBackground);

	const SDL_Color blue = { 0, 0, 255, 255 };

	m_pStartLabel = new Label("START SCENE", "Playbill", 100, blue, glm::vec2(500.0f, 120.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);

	m_pInstructionsLabel = new Label("Press 1 to Play", "Playbill", 50, blue, glm::vec2(500.0f, 190.0f));
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);

	m_pShip = new Ship();
	m_pShip->getTransform()->position = glm::vec2(500.0f, 300.0f);
	addChild(m_pShip);

	// Start Button
	m_pStartButton = new Button();
	m_pStartButton->getTransform()->position = glm::vec2(500.0f, 500.0f);

	m_pStartButton->addEventListener(CLICK, [&]()-> void
	{
		m_pStartButton->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
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


}