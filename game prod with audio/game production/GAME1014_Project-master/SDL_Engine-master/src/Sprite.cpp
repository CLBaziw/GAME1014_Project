#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
       = default;

void Sprite::DeactivateSprite()
{
	setActive(false);
	getTransform()->position = glm::vec2(0, 0);
}

SpriteSheet* Sprite::getSpriteSheet()
{
	return m_pSpriteSheet;
}

Animation& Sprite::getAnimation(const std::string& name)
{
	return m_pAnimations[name];
}

void Sprite::setSpriteSheet(SpriteSheet* sprite_sheet)
{
	m_pSpriteSheet = sprite_sheet;
}

void Sprite::setAnimation(const Animation& animation)
{
	if(!m_animationExists(animation.name))
	{
		m_pAnimations[animation.name] = animation;
	}
}

bool Sprite::m_animationExists(const std::string& id)
{
	return m_pAnimations.find(id) != m_pAnimations.end();
}
