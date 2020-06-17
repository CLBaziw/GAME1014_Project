#pragma once
#ifndef __PLANE__
#define __PLANE__

#include "Sprite.h"

class Platform final : public Sprite
{
public:
	Platform();
	~Platform();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;


private:
	void m_buildAnimations();
};

#endif /* defined (__PLANE__) */
