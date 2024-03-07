#pragma once
#include "SpriteGo.h"

class TileMap;
class SceneGame;

enum class MoveState
{
	STOP,
	UP,
	DOWN,
	RIGHT,
	LEFT,
	COUNT,
};

enum class ItemMode
{
	NONE,
	POWER_COOKIE,
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
	std::stack<sf::Vector2i> inputDirections;
	sf::Vector2f currentPos;
	sf::Vector2f nextPos;

	ItemMode itemMode = ItemMode::NONE;
	float itemDuration = 0.f;
	float itemTimer = 0.f;

	TileMap* tileMap = nullptr;
	SceneGame* sceneGame = nullptr;

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

	const ItemMode GetItemMode() const { return itemMode; }
	void SetItemMode(ItemMode mode);
	void OnDie();
};

