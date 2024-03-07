#pragma once
#include "SpriteGo.h"

class TileMap;

class Ghost : public SpriteGo
{
protected:
	sf::Vector2f direction = { 1.f, 0.f };
	float speed = 300.f;
	float timer = 0.f;
	float moveTime = 0.f;

	sf::Vector2i gridIndex = { 8, 2 };
	sf::Vector2f currentPos;
	sf::Vector2f nextPos;

	TileMap* tileMap;

public:
	Ghost(const std::string& name = "");
	~Ghost() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

