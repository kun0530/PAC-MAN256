#pragma once
#include "Ghost.h"

class GhostPinky : public Ghost
{
private:
	bool isMove = false;

public:
	GhostPinky(const std::string& name = "");
	~GhostPinky() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void DecideDirection() override;

	void CornerMove(std::vector<sf::Vector2f>& directions) override;
	void ForkMove(std::vector<sf::Vector2f>& directions) override;
};
