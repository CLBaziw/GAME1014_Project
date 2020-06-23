#pragma once
#ifndef __BOX__
#define __BOX__

#include "Sprite.h"

class Box
{
private:
	int m_x, m_y;
	Sprite* m_sprite;
	SDL_Rect m_rect;

public:
	Box(int x, int y, bool hasSprite = false);
	~Box();

	void Update();
	void Render();
	int GetX();
	Sprite* GetSprite();

	Sprite* GetRandomObstacle(int x, int y);
};
#endif