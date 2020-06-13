#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#define GRAV 1.6
#define JUMPFORCE 24.0

#include "PlayerAnimationState.h"
#include "Sprite.h"

class Player final : public Sprite
{
public:
	Player();
	~Player();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	// setters
	void setAnimationState(PlayerAnimationState new_state);
	void setPosition(int x, int y);

	// Movement
	void Stop();
	void StopX();
	void StopY();
	void SetAccelX(double a);
	void SetAccelY(double a);
	bool isJumping();
	void SetJumping(bool j);
	double GetVelX();
	double GetVelY();


private:
	void m_buildAnimations();
	PlayerAnimationState m_currentAnimationState;

	bool m_jumping;
	double m_accelX, m_accelY, m_velX, m_maxVelX, m_velY, m_maxVelY, m_drag, m_grav;
};

#endif /* defined (__PLAYER__) */