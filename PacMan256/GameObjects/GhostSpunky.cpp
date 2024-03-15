#include "pch.h"
#include "GhostSpunky.h"
#include "player.h"
#include "TileMap.h"

GhostSpunky::GhostSpunky(const std::string& name) : Ghost(name)
{
}

void GhostSpunky::Init()
{
	Ghost::Init();

	originTextureId = "graphics/Ghost_Black.png";
	SetTexture(originTextureId);
}

void GhostSpunky::Release()
{
	Ghost::Release();
}

void GhostSpunky::Reset()
{
	Ghost::Reset();
}

void GhostSpunky::Update(float dt)
{
	Ghost::Update(dt);
}

void GhostSpunky::FixedUpdate(float dt)
{
	Ghost::FixedUpdate(dt);
}

void GhostSpunky::Draw(sf::RenderWindow& window)
{
	Ghost::Draw(window);
}

void GhostSpunky::DecideDirection()
{
	sf::Vector2f playerPostion = player->GetPosition();
	float distance = Utils::Magnitude(playerPostion - position);

	if (distance <= 500.f)
	{
		Ghost::DecideDirection();
	}
	else if (timer == 0.f)
	{
		direction = { 0.f, 0.f };
	}
}

void GhostSpunky::CornerMove(std::vector<sf::Vector2f>& directions)
{
	Ghost::CornerMove(directions);
}

void GhostSpunky::ForkMove(std::vector<sf::Vector2f>& directions)
{
	Ghost::ForkMove(directions);
}
