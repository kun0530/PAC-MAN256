#pragma once
#include "GameObject.h"

class SceneGame;

class DeathEffectGo : public GameObject
{
private:
	SceneGame* sceneGame = nullptr;

	float effectTimer = 0.f;
	float effectDuration = 1.f;
	float rectSpeed = 300.f;

	sf::Color effectColor = sf::Color::Black;
	sf::Vector2f initPosition = { 0.f, 0.f };

	std::vector<std::pair<sf::RectangleShape*, sf::Vector2f>> rects;

public:
	DeathEffectGo(const std::string& name = "");
	~DeathEffectGo() override = default;

	void SetEffectColor(sf::Color color) { effectColor = color; }
	void SetInitPosition(sf::Vector2f pos) { initPosition = pos; }

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

