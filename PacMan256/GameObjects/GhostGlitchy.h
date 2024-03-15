#pragma once
#include "Ghost.h"
class GhostGlitchy : public Ghost
{
private:
	float warpTimer = 0.f;
	float warpDuration = 5.f;

public:
	GhostGlitchy(const std::string& name = "");
	~GhostGlitchy() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void CornerMove(std::vector<sf::Vector2f>& directions) override;
	void ForkMove(std::vector<sf::Vector2f>& directions) override;
};

