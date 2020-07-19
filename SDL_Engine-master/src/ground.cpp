#include "ground.h"
#include "Game.h"
#include "TextureManager.h"

ground::ground(float x, float y)
{
	if (TheGame::Instance()->getLevel() == 0)
	{
		TextureManager::Instance()->load("../Assets/plat/ground.png", "ground");
	}
	else
	{
		TextureManager::Instance()->load("../Assets/plat/ground2.png", "ground");


	}
	//TextureManager::Instance()->load("../Assets/plat/ground.png", "ground");

	auto size = TextureManager::Instance()->getTextureSize("ground");
	//auto size = TextureManager::Instance()->getTextureSize("ground2");
	setWidth(size.x);
	setHeight(size.y);

	//getTransform()->m_dst.w = size.x;
	//getTransform()->m_dst.h = size.y;

	getTransform()->position = glm::vec2(x, y);
	getTransform()->m_dst.x = x;
	getTransform()->m_dst.y = y;

	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
}

ground::~ground()
{
}

void ground::draw()
{
	
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;	
	
	//const auto x = getTransform()->m_dst.x;
	//const auto y = getTransform()->m_dst.y;

	TextureManager::Instance()->draw("ground", x, y, 0, 255, false);
}

void ground::update()
{
}

void ground::clean()
{
}

