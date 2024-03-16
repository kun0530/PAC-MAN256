#include "pch.h"
#include "GhostClyde.h"

GhostClyde::GhostClyde(const std::string& name) : Ghost(name)
{
}

void GhostClyde::Init()
{
	Ghost::Init();

	originTextureId = "graphics/Ghost_Orange.png";
	SetTexture(originTextureId);

	color = sf::Color::Color(255, 127, 0);
}

void GhostClyde::Release()
{
	Ghost::Release();
}

void GhostClyde::Reset()
{
	Ghost::Reset();
}

void GhostClyde::Update(float dt)
{
	Ghost::Update(dt);
}

void GhostClyde::FixedUpdate(float dt)
{
	Ghost::FixedUpdate(dt);
}

void GhostClyde::Draw(sf::RenderWindow& window)
{
	Ghost::Draw(window);
}

void GhostClyde::CornerMove(std::vector<sf::Vector2f>& directions)
{
	Ghost::CornerMove(directions);
}

void GhostClyde::ForkMove(std::vector<sf::Vector2f>& directions)
{
	for (auto dir : directions)
	{
		if (dir == sf::Vector2f(0.f, 1.f))
		{
			direction = dir;
			return;
		}
	}
	Ghost::ForkMove(directions);
}
