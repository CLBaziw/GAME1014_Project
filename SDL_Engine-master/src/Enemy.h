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
	void setAnimationState(EnemyAnimationState new_state);
	void setPosition(int x, int y);
	
private:
	void m_buildAnimations();
	EnemyAnimationState m_currentAnimationState;
};

#endif /* defined (__ENEMY__) */