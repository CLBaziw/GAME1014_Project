#include "EndScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"


EndScene::EndScene()
{
	EndScene::start();
}

EndScene::~EndScene()
{
	SoundManager::Instance().stopMusic();
}

void EndScene::draw()
{
	drawDisplayList();
}

void EndScene::update()
{
	updateDisplayList();
}

void EndScene::clean()
{
	delete m_label;

	delete m_pRestartButton;
	m_pRestartButton = nullptr;

	removeAllChildren();
}

void EndScene::handleEvents()
{
	EventManager::Instance().update();

	// Button Events
	m_pRestartButton->update();

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

void EndScene::start()
{
	m_pBackground = new Background("../Assets/backgrounds/startscene.png", "startscene-background", BACKGROUND, glm::vec2(0, 0), true);
	addChild(m_pBackground);

	const SDL_Color Metallicgold = { 212,175, 55, 0 };
	m_label = new Label("END SCENE", "galaxy_1", 100, Metallicgold, glm::vec2(576.0f, 120.0f));
	m_label->setParent(this);
	addChild(m_label);


	//Score Board
	const SDL_Color color = { 225, 180, 0, 225 };
	m_pScoreBoard = new ScoreBoard("Score:" + std::to_string(PlayerScore), "Playbill", 150, color, glm::vec2(576.0f, 250.0f));
	m_pScoreBoard->setParent(this);
	addChild(m_pScoreBoard);

	// Restart Button
	m_pRestartButton = new Button("../Assets/Menu Asset/play-again.png", "restartButton", RESTART_BUTTON);
	m_pRestartButton->getTransform()->position = glm::vec2(300.0f, 450.0f);
	m_pRestartButton->addEventListener(CLICK, [&]()-> void
	{
		m_pRestartButton->setActive(false);
		TheGame::Instance()->changeSceneState(LEVELSELECT_SCENE);
	});

	m_pRestartButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pRestartButton->setAlpha(128);
	});

	m_pRestartButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pRestartButton->setAlpha(255);
	});

	addChild(m_pRestartButton);

	//Exit button
	m_pExitButton = new Button("../Assets/Menu Asset/Exit_BTN.png", "exitButton", EXIT_BUTTON);
	m_pExitButton->getTransform()->position = glm::vec2(852.0f, 450.0f);
	m_pExitButton->addEventListener(CLICK, [&]()-> void
	{
		m_pExitButton->setActive(false);
		TheGame::Instance()->quit();
	});

	m_pExitButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pExitButton->setAlpha(128);
	});

	m_pExitButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pExitButton->setAlpha(255);
	});

	addChild(m_pExitButton);

	SoundManager::Instance().playSound("endScreen");
}