#pragma once
#ifndef __ENEMY__
#define __ENEMY__

#include "EnemyAnimationState.h"
#include "Sprite.h"

class Enemy final : public Sprite
{
public:
	Enemy();
	~Enemy();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	// Setters
	/*void setAnimationState(EnemyAnimationState new_state);*/
	void setPosition(int x, int y);

	void SetX(float x);

	void SetY(float y);

	// Shooting
	bool isShooting();
	void SetShooting(bool s);

	void setDst();
	SDL_FRect& getDst();
	
private:
	void m_buildAnimations();
	SDL_FRect dst;
	/*EnemyAnimationState m_currentAnimationState;*/

	bool m_shooting;
	float m_maxVelX, m_maxVelY, m_drag, m_grav;
	int maxHp = 640; // set Enemy hit point
	int currentHp = 350;
};

#endif /* defined (__ENEMY__) */
