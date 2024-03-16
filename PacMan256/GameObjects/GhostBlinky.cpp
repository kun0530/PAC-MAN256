#include "pch.h"
#include "GhostBlinky.h"
#include "Player.h"
#include "TileMap.h"

GhostBlinky::GhostBlinky(const std::string& name) : Ghost(name)
{
}

void GhostBlinky::Init()
{
	Ghost::Init();

	originTextureId = "graphics/Ghost_Red.png";
	SetTexture(originTextureId);

	color = sf::Color::Red;
}

void GhostBlinky::Release()
{
	Ghost::Release();
}

void GhostBlinky::Reset()
{
	Ghost::Reset();
}

void GhostBlinky::Update(float dt)
{
	Ghost::Update(dt);
}

void GhostBlinky::FixedUpdate(float dt)
{
	Ghost::FixedUpdate(dt);
}

void GhostBlinky::Draw(sf::RenderWindow& window)
{
	Ghost::Draw(window);
}

void GhostBlinky::CornerMove(std::vector<sf::Vector2f>& directions)
{
	Ghost::CornerMove(directions);
}

void GhostBlinky::ForkMove(std::vector<sf::Vector2f>& directions)
{
	Ghost::ForkMove(directions);
}
