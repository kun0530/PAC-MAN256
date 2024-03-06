#pragma once

class TileMap;
class Player;

class SceneGame : public Scene
{
protected:
	TileMap* tileMap = nullptr;
	Player* player = nullptr;

public:
	SceneGame(SceneIds id);
	virtual ~SceneGame();

	void Init() override;
	void Release() override;

	void Enter() override;
	void Exit() override;

	void Update(float dt) override;

	void Draw(sf::RenderWindow& window) override;
};