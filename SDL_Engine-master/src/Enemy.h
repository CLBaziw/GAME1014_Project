#pragma once
#include "Sprite.h"

#ifndef __ALIEN__
#define __ALIEN__

class Enemy final : public Sprite 
{
public:
	Enemy();
	~Enemy();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	int getDamage() { return damage; }

private:
	void m_buildAnimations();
	int damage = 50;
};

#endif /* defined (__ALIEN__) */