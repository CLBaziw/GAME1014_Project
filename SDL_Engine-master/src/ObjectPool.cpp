#include "ObjectPool.h"

void ObjectPool::CreateObjectPool()
{
}

void ObjectPool::UpdateActiveSprites()
{
	
	for (int i = 0; i < m_activeSprites.size(); i++)
	{
		bool active = m_activeSprites[i]->getActive();
		if (!active) // Deactivate Inactive Sprites
		{
			m_activeSprites.erase(m_activeSprites.begin() + i);
		}
		else
		{
			m_activeSprites[i]->update();
			m_activeSprites[i]->draw();
		}
	}
}

void ObjectPool::AddToActiveList(GameObjectType newObj)
{
	GameObject* temp;

	switch (newObj)
	{
	case PLATFORM:
		// Search through PlatformPool for an inactive platform
		// temp = first inactive platform
	case ENEMY:
		// Search through EnemyPool for an inactive enemy
		// temp = first inactive enemy
	// OBSTACLE DIFFERENCES NEED TO BE CHANGED TO ONLY BE SPIKES, ETC INSTEAD OF HAVING THEIR OWN ENUM
	case PLAYER:
		// temp = player;
	}

	m_activeSprites.push_back(temp);
}
