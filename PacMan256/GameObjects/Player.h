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
	float moveTime = 0.f;
	float speed = 300.f;

	sf::Vector2i playerGridIndex = { 13, 16 };
	MoveState moveState = MoveState::STOP;
	sf::Vector2f currentPos;
	sf::Vector2f nextPos;

	TileMap* tileMap;

public:
	Player(const std::string& name = "");
	~Player() override = default;

	bool IsMove(sf::Vector2i dir);
	const sf::Vector2i& GetGridIndex() { return playerGridIndex; }

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void OnDie();
};

