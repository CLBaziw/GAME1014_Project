#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "Button.h"
#include "Bullet.h"
//#include "EnemyBullet.h"
#include "Background.h"
#include "Platform.h"
#include "ground.h"
#include "Obstacle.h"
#include "Scoreboard.h"




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
	/*void EnemyShoot();*/
	

private:
	glm::vec2 m_mousePosition;

	Player* m_pPlayer;
	Enemy* m_pEnemy;
	Background* m_pBackground;
	Platform* m_platform;
	ground* m_ground;
	Obstacle* m_pObstacle1;
	Obstacle* m_pObstacle2;
	Obstacle* m_pObstacle3;
	ScoreBoard* m_pScoreBoard;
	//SDL_FRect* c_pPlayer;
	//SDL_FRect* c_platform;

	bool m_playerFacingRight;
	/*bool m_enemyFacingRight;*/

	std::vector<Bullet*> m_pPlayerBulletVec;
	/*std::vector<EnemyBullet*> m_pEnemyBulletVec;*/

	Button* m_pPauseButton;
	Button* m_pContinueButton;
	

	float m_shootTime;
	float m_currentTime;

	// movement for character
	int moveX = 0;
	int moveY = 0;
	int moveSpeed = 5;

};

#endif /* defined (__PLAY_SCENE__) */