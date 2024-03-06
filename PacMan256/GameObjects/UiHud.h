#pragma once
#include "GameObject.h"
#include "SpriteGo.h"
#include "TextGo.h"

class UiHud : public GameObject
{
protected:
	sf::Vector2f referenceResolution = { 1920, 1080 };
	sf::Vector2f resolution = referenceResolution;

	std::string formatScore = "SCORE: ";
	std::string formatHighScore = "HIGH SCORE: ";
	// std::string formatAmmo = "{0}/{1}";
	std::string formatWave = "WAVE: ";
	std::string formatZombieCount = "ZOMBIES: ";
	std::string formatFps = "FPS: ";


	TextGo textScore;
	TextGo textHiScore;

	SpriteGo imgAmmoIcon;
	TextGo textAmmo;
	sf::RectangleShape gaugeHp;
	TextGo textWave;
	TextGo textZombieCount;

	TextGo textMessage;

	TextGo textFps;

	sf::Vector2f gaugeHpSize = { 500.f, 50.f };

public:
	UiHud(const std::string& name = "");
	~UiHud() override = default;

	void SetResolution(const sf::Vector2f resolution);

	void SetScore(int s);
	void SetHiScore(int s);

	void SetAmmo(int current, int total);
	void SetHp(int hp, int max);
	void SetWave(int w);
	void SetZombieCount(int count);

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