#pragma once
#include "Scene.h"
class SceneTitle : public Scene
{
public:
	SceneTitle(SceneIds id);
	virtual ~SceneTitle();

	void Init() override;
	void Release() override;
	void Enter() override;
	void Exit() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

