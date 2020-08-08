#pragma once
#ifndef __TRANSFORM__
#define __TRANSFORM__
#include <glm/vec2.hpp>
#include "SDL.h"
struct Transform
{
	glm::vec2 position;
	glm::vec2 rotation;
	glm::vec2 scale;
	SDL_FRect m_dst;
};
#endif /* defined (__TRANSFORM__) */