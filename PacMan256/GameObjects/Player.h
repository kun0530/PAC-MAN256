#pragma once
#include "SpriteGo.h"

class TileMap;
class SceneGame;

class Player : public SpriteGo
{
protected:
	sf::Vector2f direction = { 0.f, 0.f };
	float timer = 0.f;
	float moveTime = 0.f;
	float speed = 250.f;

	TileMap* currentTileMap = nullptr;
	sf::Vector2i gridIndex = { 13, 16 };
	int currentTileMapId = 0;

	bool isArrive = true;
	bool isWarp = false;
	std::queue<sf::Vector2i> inputDirections;
	sf::Vector2f currentPos;
	sf::Vector2f nextPos;

	ItemType usingItem = ItemType::NONE;
	float itemDuration = 0.f;
	float itemTimer = 0.f;

	
	SceneGame* sceneGame = nullptr;

public:
	Player(const std::string& name = "");
	~Player() override = default;

	// bool IsMove(sf::Vector2i dir);
	const sf::Vector2i& GetGridIndex() { return gridIndex; }

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetCurrentTileMapId(int id) { currentTileMapId = id; }
	const int GetCurrentTileMapId() const { return currentTileMapId; }

	bool EatItem();
	const ItemType GetUsingItem() const { return usingItem; }
	void SetUsingItem(ItemType item);
	void EndUsingItem();
	void OnDie();
};