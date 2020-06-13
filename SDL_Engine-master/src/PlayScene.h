#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Enemy.h"
#include "Button.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	void CheckBounds();
	void checkCollision();

private:
	glm::vec2 m_mousePosition;

	Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	Enemy* m_pAlien;

	bool m_playerFacingRight;

	Button* m_pPauseButton;
	Button* m_pContinueButton;
	Button* m_pNextButton;

	// movement for character
	int moveX = 0;
	int moveY = 0;
	int moveSpeed = 5;

};

#endif /* defined (__PLAY_SCENE__) */