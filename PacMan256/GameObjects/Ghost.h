#pragma once
#include "SpriteGo.h"

class TileMap;
class Player;

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

	TileMap* tileMap = nullptr;
	Player* player = nullptr;

public:
	Ghost(const std::string& name = "");
	~Ghost() override = default;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void FixedUpdate(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void ChangeMode();
	void OnDie();
};

