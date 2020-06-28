#include "Platform.h"
#include "TextureManager.h"

Platform::Platform(float x, float y)
{
	// Copy from obstacle but make relevant for platform
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/plat/plas.txt",
		"../Assets/plat/plas.png",
		"platform"
	);



	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("platform"));
	setType(PLATFORM);
	
	setWidth(128);
	setHeight(128);

	getTransform()->position = glm::vec2(x, y);

	//getTransform()->m_dst.x = x;
	//getTransform()->m_dst.y = y;
	//getTransform()->m_dst.w = 128.0f;
	//getTransform()->m_dst.h = 128.0f;

	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;


	m_buildAnimations();
}

Platform::~Platform()
= default;

void Platform::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;	
	
	//const auto x = getTransform()->m_dst.x;
	//const auto y = getTransform()->m_dst.y;

	//draw the bullet according to animation state
	
	TextureManager::Instance()->playAnimation("platform", getAnimation("stationary-platform"),
			x, y, 0.12f, 0, 255, true);
}

void Platform::update()
{

}

void Platform::clean()
{
}

void Platform::setDst()
{
	dst.x = getTransform()->position.x;
	dst.y = getTransform()->position.y;
	dst.w = getWidth();
	dst.h = getHeight();

}

SDL_FRect& Platform::getDst()
{
	return dst;
}

void Platform::m_buildAnimations()
{
	Animation platform = Animation();

	platform.name = "stationary-platform";
	platform.frames.push_back(getSpriteSheet()->getFrame("plas"));
	//platform.frames.push_back(getSpriteSheet()->getFrame("plat-2"));
	//platform.frames.push_back(getSpriteSheet()->getFrame("plat-3"));
	//platform.frames.push_back(getSpriteSheet()->getFrame("hazard"));

	setAnimation(platform);
}