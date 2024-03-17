#pragma once
#include "Scene.h"

class SpriteGo;
class TextGo;

class SceneTitle : public Scene
{
private:
	sf::RectangleShape title;

	SpriteGo* logo = nullptr;
	TextGo* textMessage = nullptr;

	const int themeCount = 5;
	std::vector<SpriteGo*> themes;
	int prevThemeNum = 0;
	float imageSpeed = 2000.f;
	float imageTimer = 0.f;
	float imageMoveTime = 0.f;
	sf::Vector2f imageDirection = { 0.f, 0.f };
	bool isMovingImage = false;

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

