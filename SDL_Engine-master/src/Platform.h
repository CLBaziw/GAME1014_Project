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

	void setDst();
	SDL_FRect& getDst();

	void setDst();
	SDL_FRect& getDst();

private:
	void m_buildAnimations();
	SDL_FRect dst;
};

#endif /* defined (__PLANE__) */
