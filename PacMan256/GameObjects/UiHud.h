#pragma once
#include "GameObject.h"
#include "SpriteGo.h"
#include "TextGo.h"

// class Player;

class UiHud : public GameObject
{
protected:
	// Player* player = nullptr;

	sf::Vector2f referenceResolution = { 1920, 1080 };
	sf::Vector2f resolution = referenceResolution;

	std::string formatFps = "FPS: ";

	TextGo textScore;
	TextGo textHiScore;

	/*TextGo textChain;
	float textChainTimer = 0.f;*/

	TextGo textMessage;

	TextGo textFps;

	// sf::Vector2f gaugeHpSize = { 500.f, 50.f };

public:
	UiHud(const std::string& name = "");
	~UiHud() override = default;

	void SetResolution(const sf::Vector2f resolution);

	void SetScore(int score);
	void SetHighScore(int hiScore);
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