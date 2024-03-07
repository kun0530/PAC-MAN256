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

	sf::Font& font = RES_MGR_FONT.Get("fonts/Arial.ttf");
	TextGo* uiScore = nullptr;
	std::vector<TextGo*> posTexts;

	std::list<GameObject*> ghostList;

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
	const std::list<GameObject*>& GetGhostList() const { return ghostList; }
};