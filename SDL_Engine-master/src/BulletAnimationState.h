#pragma once
#ifndef __BULLET_ANIMATION_STATE__
#define __BULLET_ANIMATION_STATE__

enum BulletAnimationState
{
	P_BULLET_MOVE_RIGHT,
	P2_BULLET_MOVE_RIGHT,
	E_BULLET_MOVE_RIGHT,
	E2_BULLET_MOVE_RIGHT,
	P_BULLET_MOVE_LEFT,
	P2_BULLET_MOVE_LEFT,
	E_BULLET_MOVE_LEFT,
	E2_BULLET_MOVE_LEFT,
	BULLET_CRASH_RIGHT,
	BULLET_CRASH_LEFT,
	NUM_OF_BULLET_ANIMATION_STATES
};

enum BulletType
{
	ENEMY_BULLET,
	ENEMY_BULLET2,
	PLAYER_BULLET,
	PLAYER_BULLET2,
	NUM_OF_BULLET_TYPE
};
#endif