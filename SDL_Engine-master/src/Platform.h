#pragma once
#ifndef __PLANE__
#define __PLANE__

#include "Sprite.h"


class Platform final : public Sprite
{
public:
	Platform(float x, float y);
	~Platform();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	int getDamage() { return damage; }

	void setDst();
	SDL_FRect& getDst();

private:
	void m_buildAnimations();
	int damage = 50;
	SDL_FRect dst;
};

#endif /* defined (__PLANE__) */
