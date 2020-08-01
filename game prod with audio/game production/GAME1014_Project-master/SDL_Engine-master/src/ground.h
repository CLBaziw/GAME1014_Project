#pragma once
#include "Sprite.h"
class ground : public Sprite
{
public:
	ground(float x, float y);
	~ground();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void setDst();
	SDL_FRect& getDst();

private:
	//void m_buildAnimations();
	SDL_FRect dst;
};

