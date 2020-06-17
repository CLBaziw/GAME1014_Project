#pragma once
#ifndef __PLANE__
#define __PLANE__

#include "Sprite.h"

class Plane final : public Sprite
{
public:
	Plane();
	~Plane();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	int getDamage() { return damage; }

private:
	void m_buildAnimations();
	int damage = 50;
};

#endif /* defined (__PLANE__) */
