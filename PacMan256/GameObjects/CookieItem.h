#pragma once
#include "Item.h"

class SceneGame;
class TileMap;
class Player;

class CookieItem : public Item
{
protected:
	SceneGame* sceneGame = nullptr;
	TileMap* tileMap = nullptr;
	Player* player = nullptr;

public:
	CookieItem(const std::string& name = "");
	~CookieItem() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};