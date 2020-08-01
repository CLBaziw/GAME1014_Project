#pragma once
#ifndef __ENEMY_BULLET__
#define __ENEMY_BULLET__

#include "Sprite.h"
#include "EnemyBulletAnimationState.h"
#include <SDL\include\SDL_rect.h>


class EnemyBullet : public Sprite
{
public:
	EnemyBullet(float xPos, float yPos, bool bulletType, EnemyBulletAnimationState bulletState);
	/*EnemyBullet(float xPos, float yPos, bool bulletType, EnemyBulletAnimationState bulletState);*/
	~EnemyBullet();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	// Setters
	void setAnimationState(EnemyBulletAnimationState new_state);
	void setPosition(int x, int y);
private:
	int m_speed;
	bool m_active;
	bool m_EnemyBullet;

	SDL_Rect m_rDst;

	void m_buildAnimations();
	EnemyBulletAnimationState m_currentAnimationState;
	
};
#endif
