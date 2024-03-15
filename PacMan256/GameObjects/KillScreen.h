#pragma once
#include "SpriteGo.h"

class SceneGame;
class TileMap;

class KillScreen : public SpriteGo
{
private:
	SceneGame* sceneGame = nullptr;
	TileMap* tileMap = nullptr;

	int tileMapId = 0;
	int gridIndexY = 0;
	int nextGridindexY = 0;

	sf::Vector2f currentPosition = { 0.f, 0.f };
	sf::Vector2f nextPosition = { 0.f, 0.f };

	float speed = 50.f;
	const sf::Vector2f direction = { 0.f, -1.f };

	float timer = 0.f;
	float moveTime = 0.f;

public:
	KillScreen(const std::string& name = "");
	~KillScreen() override = default;

	const int GetTileMapId() const { return tileMapId; }
	const int GetGridIndexY() const { return gridIndexY; }

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

