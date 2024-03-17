#pragma once
#include "Ghost.h"
class GhostGlitchy : public Ghost
{
private:
	bool isMove = true;
	float moveTimer = 0.f;
	float moveDuration = 5.f;

	bool isWarpStart = false;
	float warpStartTimer = 0.f;
	float warpStartDuration = 1.f;

	bool isWarpEnd = false;
	float warpEndTimer = 0.f;
	float warpEndDuration = 1.f;

	std::vector<SpriteGo*> glitchParts;

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

	void RePosition();
	void WarpAnimation(float dt);
};

