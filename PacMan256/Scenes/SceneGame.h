#pragma once

class TileMap;
class Player;
class Ghost;

class TextGo;

class SceneGame : public Scene
{
protected:
	int score = 0;
	int scoreScale = 1;

	TileMap* tileMap = nullptr;
	Player* player = nullptr;
	Ghost* ghost = nullptr;

	TextGo* uiScore = nullptr;

	std::vector<TextGo*> posTexts;

	sf::Font& font = RES_MGR_FONT.Get("fonts/Arial.ttf");

public:
	SceneGame(SceneIds id);
	virtual ~SceneGame();

	void Init() override;
	void Release() override;

	void Enter() override;
	void Exit() override;

	void Update(float dt) override;

	void Draw(sf::RenderWindow& window) override;

	void AddScore(const int score);
};