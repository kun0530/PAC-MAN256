#pragma once
#include "GameObject.h"
#include "SpriteGo.h"
#include "TextGo.h"

class SceneGame;
class Player;

class UiHud : public GameObject
{
protected:
	SceneGame* sceneGame = nullptr;
	Player* player = nullptr;

	sf::Vector2f referenceResolution = { 1920, 1080 };
	sf::Vector2f resolution = referenceResolution;

	std::string formatFps = "FPS: ";
	std::string formatMultiplier = "X";

	TextGo textScore;
	TextGo textMultiplier;
	TextGo textHiScore;

	TextGo textGame;
	TextGo textOver;
	float textGameOverInterval = 15.f;
	sf::Vector2f gameOverPosition;
	float gameOverTimer = 0.f;
	float gameOverMoveTime = 0.f;
	float gameOverSpeed = 1000.f;
	bool isGameOver = false;

	/*TextGo textChain;
	float textChainTimer = 0.f;*/

	TextGo textMessage;

	TextGo textFps;

	sf::RectangleShape itemTimer;
	sf::Vector2f itemTimerSize = { 200.f, 10.f };
	bool isUsingItem = false;

public:
	UiHud(const std::string& name = "");
	~UiHud() override = default;

	void SetResolution(const sf::Vector2f resolution);

	void SetItemTimer(float timer, float duration);
	void SetScore(int score);
	void SetMultiplier(int multiplier);
	void SetMultiplierActive(bool active);
	void SetHighScore(int hiScore);
	void SetGameOver(const bool isOver);
	// void SetChain(int chain);
	void SetMessage(const std::string& msg);
	void SetMessageActive(bool active);

	void SetFps(int fps);

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};