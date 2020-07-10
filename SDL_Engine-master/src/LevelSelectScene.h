#pragma once
#ifndef __LEVELSELECT_SCENE__
#define __LEVELSELECT_SCENE__

#include "Scene.h"
#include "Label.h"
#include "ship.h"
#include "Button.h"
#include "Background.h"
#include "SoundManager.h"

class LevelSelectScene final : public Scene
{
public:
	LevelSelectScene();
	~LevelSelectScene();

	// Inherited via Scene
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

private:
	Label* m_pLevelSelectLabel{};
	
	Background* m_pBackground;

	Button* m_pLevelOneButton;
	Button* m_pLevelTwoButton;
};

#endif /* defined (__LEVELSELECT_SCENE__) */
