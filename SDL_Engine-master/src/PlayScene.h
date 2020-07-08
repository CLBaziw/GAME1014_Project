#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Player.h"
#include "Button.h"
#include "Bullet.h"
//#include "EnemyBullet.h"
#include "Background.h"
#include "Box.h"
#include <vector>
#include "ground.h"
#include "Obstacle.h"
#include "Scoreboard.h"
#include "ObjectPool.h"

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
	void EnemyShoot();

private:
	glm::vec2 m_mousePosition;

	Player* m_pPlayer;
	Background* m_pBackground;
	ground* m_ground;
	ScoreBoard* m_pScoreBoard;

	// Boxes for scrolling
	ObjectPool* m_objPool;
	std::vector<Box*> m_vec;
	std::vector<Obstacle*> m_pObstacles;
	int m_numSpaces;

	bool m_playerFacingRight;
	/*bool m_enemyFacingRight;*/

	std::vector<Bullet*> m_pPlayerBulletVec;
	std::vector<Bullet*> m_pEnemyBulletVec;

	Button* m_pPauseButton;
	Button* m_pContinueButton;
	

	// movement for character
	int moveX = 0;
	int moveY = 0;
	int moveSpeed = 10;

	int m_bulletTimer = 0,
		m_timerMax = 35;

};

#endif /* defined (__PLAY_SCENE__) */