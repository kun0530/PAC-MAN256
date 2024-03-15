#pragma once
#include "Ghost.h"
class GhostClyde : public Ghost
{
private:

public:
	GhostClyde(const std::string& name = "");
	~GhostClyde() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void CornerMove(std::vector<sf::Vector2f>& directions) override;
	void ForkMove(std::vector<sf::Vector2f>& directions) override;
};

