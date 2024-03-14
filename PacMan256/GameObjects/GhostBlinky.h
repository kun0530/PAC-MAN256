#pragma once
#include "Ghost.h"
class GhostBlinky : public Ghost
{
private:

public:
	GhostBlinky(const std::string& name = "");
	~GhostBlinky() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void CornerMove(std::vector<sf::Vector2f>& directions) override;
	void ForkMove(std::vector<sf::Vector2f>& directions) override;
};

