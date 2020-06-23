#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "Button.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Background.h"
#include "Platform.h"

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

	void PlayerShoot();

private:
	glm::vec2 m_mousePosition;

	Player* m_pPlayer;
	Enemy* m_pEnemy;
	Background* m_pBackground;
	Platform* m_platform;
	//SDL_FRect* c_pPlayer;
	//SDL_FRect* c_platform;

	bool m_playerFacingRight;

	std::vector<Bullet*> m_pPlayerBulletVec;

	Button* m_pPauseButton;
	Button* m_pContinueButton;
	Button* m_pNextButton;

	float m_shootTime;
	float m_currentTime;

	// movement for character
	int moveX = 0;
	int moveY = 0;
	int moveSpeed = 5;

};

#endif /* defined (__PLAY_SCENE__) */