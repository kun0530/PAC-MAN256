#pragma once

class TileMap;
class SpriteGo;

class SceneDev : public Scene
{
protected:
	TileMap* tileMap = nullptr;
	SpriteGo* tile = nullptr;

public:
	SceneDev(SceneIds id);
	virtual ~SceneDev();

	void Init() override;
	void Release() override;

	void Enter() override;
	void Exit() override;

	void Update(float dt) override;

	void Draw(sf::RenderWindow& window) override;
};