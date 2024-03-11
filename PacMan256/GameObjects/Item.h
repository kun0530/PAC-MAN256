#pragma once
#include "SpriteGo.h"

class SceneGame;
class TileMap;
class Player;

class Item : public SpriteGo
{
protected:
	SceneGame* sceneGame = nullptr;
	TileMap* tileMap = nullptr;
	Player* player = nullptr;

	sf::Vector2i gridIndex = { 0, 0 };

	ItemType itemType = ItemType::NONE;

	int value = 0;

public:
	Item(const std::string& name = "");
	~Item() override = default;

	void SetGridIndex(const int x, const int y) { gridIndex = { x, y }; }
	void SetItemType(const ItemType type) { itemType = type; }

	void SetValue(const int v) { value = v; }
	const int GetValue() const { return value; }

	void SetTileMap(TileMap* tile) { tileMap = tile; }

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void FixedUpdate(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

