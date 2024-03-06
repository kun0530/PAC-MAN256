#pragma once
#include "SpriteGo.h"

class TileMap;

enum class MoveState
{
	STOP,
	UP,
	DOWN,
	RIGHT,
	LEFT,
	COUNT,
};

class Player : public SpriteGo
{
protected:
	sf::Vector2f direction = { 0.f, 0.f };
	float timer = 0.f;
	float moveTime = 1.f;
	float speed = 500.f;

	sf::Vector2i playerGridIndex = { 11, 16 };
	MoveState moveState = MoveState::STOP;
	sf::Vector2f currentPos;
	sf::Vector2f nextPos;

	TileMap* tileMap;

public:
	Player(const std::string& name = "");
	~Player() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

