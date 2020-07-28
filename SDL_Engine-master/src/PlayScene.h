#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Player.h"
#include "Button.h"
#include "Bullet.h"
#include "Background.h"
#include "Box.h"
#include <vector>
#include "ground.h"
#include "Obstacle.h"
#include "Scoreboard.h"
#include "Enemy.h"
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

	void ScrollBgGround();
	void PlayerShoot(BulletType bulletType);
	void MakeObstacles();
	void EnemyShoot();
	void gameOver();
	//void moveBackground();

private:
	glm::vec2 m_mousePosition;

	Background* m_pBackground;
	Background* m_pBackground2;
	ScoreBoard* m_pScoreBoard;
	ScoreBoard* Health;
	ScoreBoard* GameOverText;
	Player* m_pPlayer;
	ground* m_ground;

	float GameOverTimer;
	// Boxes for scrolling
	ObjectPool* m_objPool;
	std::vector<Box*> m_vec;
	std::vector<Obstacle*> m_pObstacles;
	int m_numSpaces;
	int PlayerHealth;

	bool GameOver;
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
	bool CanFire;
	float m_bulletTimer = 0,
		m_timerMax = 100;
	float x;
	float y;
	int cooldown_specialskill =0;
};

#endif /* defined (__PLAY_SCENE__) */