#include "ground.h"
#include "TextureManager.h"

ground::ground(float x, float y)
{
	TextureManager::Instance()->load("../Assets/plat/ground.png", "ground");

	auto size = TextureManager::Instance()->getTextureSize("ground");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(x, y);
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

	TextureManager::Instance()->draw("ground", x, y, 0, 255, false);
}

void ground::update()
{
}

void ground::clean()
{
}

