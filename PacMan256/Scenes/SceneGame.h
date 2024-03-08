#pragma once

class TileMap;
class Player;
class Ghost;

class UiHud;
class TextGo;

class SceneGame : public Scene
{
protected:
	int score = 0;
	int scoreMultiplier = 1;

	int chain = 0;

	TileMap* tileMap = nullptr;
	Player* player = nullptr;
	Ghost* ghost = nullptr;

	sf::Font& font = RES_MGR_FONT.Get("fonts/Arial.ttf");
	UiHud* uiHud = nullptr;
	std::vector<TextGo*> posTexts;
	TextGo* textChain = nullptr;

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
	void AddChain();
	void ResetChain();

	const int GetScoreMultiplier() const { return scoreMultiplier; }
	void SetScoreMultiplier(int multiplier) { scoreMultiplier = multiplier; };

	const std::list<GameObject*>& GetGhostList() const { return ghostList; }
	void ChangeGhostMode();
};