#pragma once
#ifndef __OBJECT_POOL__
#define __OBJECT_POOL__

#include "GameObject.h"
#include "GameObjectType.h"
#include <vector>

class ObjectPool
{
public:
	void CreateObjectPool();
	void UpdateActiveSprites();
	void AddToActiveList(GameObjectType newObj);
private:
	std::vector<GameObject*> m_activeSprites;
};

#endif

