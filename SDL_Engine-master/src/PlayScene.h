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
#include "Box.h"
#include <vector>
#include "ground.h"
#include "Obstacle.h"

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
 
	void checkCollision();

	void PlayerShoot();

	void MakeObstacles();

private:
	glm::vec2 m_mousePosition;

	Player* m_pPlayer;
	Enemy* m_pEnemy;
	Background* m_pBackground;
	Platform* m_platform;
	std::vector<Sprite*> m_pObstacles;

	// Boxes for scrolling
	std::vector<Box*> m_vec;
	int m_numSpaces;

	ground* m_ground;
	Obstacle* m_pObstacle1;
	Obstacle* m_pObstacle2;
	Obstacle* m_pObstacle3;
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
	int moveSpeed = 10;

};

#endif /* defined (__PLAY_SCENE__) */