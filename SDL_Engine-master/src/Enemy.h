#pragma once
#ifndef __ENEMY__
#define __ENEMY__

#include "ObstacleAnimationState.h"
#include "Sprite.h"

class Enemy final : public Sprite
{
public:
	Enemy(int x, int y);
	~Enemy();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	// Setters
	void setAnimationState(ObstacleAnimationState new_state);
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

	bool m_shooting;
	float m_maxVelX, m_maxVelY, m_drag, m_grav;
	int maxHp = 640; // set Enemy hit point
	int currentHp = 350;
	ObstacleAnimationState m_currentAnimationState;
};

#endif /* defined (__ENEMY__) */
