#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "Button.h"
#include "Bullet.h"
#include "Background.h"
#include "Box.h"
#include <vector>
#include "ground.h"
#include "Obstacle.h"
#include "ObjectPool.h"
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
 
	void checkCollision();

	void PlayerShoot();

	void MakeObstacles();

private:
	glm::vec2 m_mousePosition;

	Player* m_pPlayer;
	Background* m_pBackground;
	ground* m_ground;
	Platform* m_platform;

	// Boxes for scrolling
	ObjectPool* m_objPool;
	std::vector<Box*> m_vec;
	std::vector<Obstacle*> m_pObstacles;
	int m_numSpaces;

	bool m_playerFacingRight;

	std::vector<Bullet*> m_pPlayerBulletVec;

	Button* m_pPauseButton;
	Button* m_pContinueButton;
	Button* m_pNextButton;

	// movement for character
	int moveX = 0;
	int moveY = 0;
	int moveSpeed = 10;

};

#endif /* defined (__PLAY_SCENE__) */