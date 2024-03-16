#pragma once

class TileMap;
class KillScreen;
class Player;

class UiHud;
class TextGo;

class SceneGame : public Scene
{
protected:
	int score = 0;
	int scoreMultiplier = 1;
	float fruitTimer = 0.f;
	float fruitRealDuration = 5.f;
	const float fruitDuration = 5.f;
	const float fruitAddDuration = 1.f;

	int chain = 0;

	TileMap* startTile = nullptr;
	KillScreen* killScreen = nullptr;

	std::vector<TileMap*> tileMaps;
	// std::list<TileMap*> tempTileMaps;
	int tileMapNum = 4;

	TileMap* prevTileMap = nullptr;
	TileMap* currentTileMap = nullptr;
	TileMap* nextTileMap = nullptr;
	int currentTileMapId = 0;

	Player* player = nullptr;
	// Ghost* ghost = nullptr;

	sf::Font& font = RES_MGR_FONT.Get("fonts/editundo.ttf");
	UiHud* uiHud = nullptr;
	std::vector<TextGo*> posTexts;
	TextGo* textChain = nullptr;

	std::list<GameObject*> ghostList;

	bool isGhostKill = false;
	const float zoomInSize = 0.5f;
	const float zoomOutSize = 0.78125f;
	bool isZoomIn = false;
	float cameraZoomInTimer = 0.f;
	float cameraZoomInDuration = 1.f;
	bool isZoomOut = false;
	float cameraZoomOutTimer = 0.f;
	float cameraZoomOutDuration = 1.f;

public:
	SceneGame(SceneIds id);
	virtual ~SceneGame();

	void Init() override;
	void Release() override;

	void Enter() override;
	void Exit() override;

	void Update(float dt) override;

	void Draw(sf::RenderWindow& window) override;

	TileMap* ChangeTileMap(bool isGoUp);
	TileMap* GetCurrentTileMap() const { return currentTileMap; }
	TileMap* GetPrevTileMap() const { return prevTileMap; }
	TileMap* GetNextTileMap() const { return nextTileMap; }
	const int GetCurrentTileMapId() const { return currentTileMapId; }

	int CountPathNum(int x, int y, TileMap* map, std::vector<sf::Vector2f>& directions) const;

	void AddScore(const int score);
	void AddChain();
	void ResetChain();

	const int GetScoreMultiplier() const { return scoreMultiplier; }
	void SetScoreMultiplier(int multiplier);

	const std::list<GameObject*>& GetGhostList() const { return ghostList; }
	void ChangeGhostMode();
	void CreateGhost(int num);

	void KillGhost(sf::Vector2f pos, sf::Color color);
	void MakeDeatEffect(sf::Vector2f pos, sf::Color color);
	void GameOver();
	void ZoomCamera(float dt);
};