#pragma once
#include "SpriteGo.h"

class SceneGame;
class TileMap;
class Player;

class Ghost : public SpriteGo
{
protected:
	sf::Vector2f direction = { 1.f, 0.f };
	float speed = 200.f;
	float timer = 0.f;
	float moveTime = 0.f;

	sf::Vector2i gridIndex = { 8, 2 };
	int currentTileMapId = 0;
	sf::Vector2f currentPos;
	sf::Vector2f nextPos;
	bool isWarp = false;

	SceneGame* sceneGame = nullptr;
	TileMap* tileMap = nullptr;
	Player* player = nullptr;

	std::string originTextureId;

public:
	Ghost(const std::string& name = "");
	~Ghost() override = default;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void FixedUpdate(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	virtual void DecideDirection();
	void CheckTileMapBoundary();
	virtual void CornerMove(std::vector<sf::Vector2f>& directions);
	virtual void ForkMove(std::vector<sf::Vector2f>& directions);
	sf::Vector2i BFS(const sf::Vector2i& startIndex, const sf::Vector2i& targetIndex, std::vector<sf::Vector2f>& directions);

	void ChangeMode();
	void OnDie();
};

