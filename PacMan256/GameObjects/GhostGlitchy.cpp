#include "pch.h"
#include "GhostGlitchy.h"
#include "TileMap.h"

GhostGlitchy::GhostGlitchy(const std::string& name) : Ghost(name)
{
}

void GhostGlitchy::Init()
{
	Ghost::Init();

	originTextureId = "graphics/Ghost_Glitch.png";
	SetTexture(originTextureId);
}

void GhostGlitchy::Release()
{
	Ghost::Release();
}

void GhostGlitchy::Reset()
{
	Ghost::Reset();
}

void GhostGlitchy::Update(float dt)
{
	if (warpTimer > warpDuration)
	{
		warpTimer = 0.f;
		int CountX = tileMap->GetCellCount().x;
		int CountY = tileMap->GetCellCount().y;
		do
		{
			gridIndex.x = Utils::RandomRange(0, CountX - 1);
			gridIndex.y = Utils::RandomRange(0, CountY - 1);
		} while (tileMap->IsBlocked(gridIndex.x, gridIndex.y));


		SetPosition(tileMap->GetGridPosition(gridIndex.x, gridIndex.y));
		gridIndex.x += (int)direction.x;
		gridIndex.y += (int)direction.y;
		nextPos = tileMap->GetGridPosition(gridIndex.x, gridIndex.y);
		currentPos = position;
		moveTime = Utils::Magnitude(nextPos - currentPos) / speed;
		timer = 0.f;
	}
	else
	{
		warpTimer += dt;
		Ghost::Update(dt);
	}
}

void GhostGlitchy::FixedUpdate(float dt)
{
	Ghost::FixedUpdate(dt);
}

void GhostGlitchy::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void GhostGlitchy::CornerMove(std::vector<sf::Vector2f>& directions)
{
	Ghost::CornerMove(directions);
}

void GhostGlitchy::ForkMove(std::vector<sf::Vector2f>& directions)
{
	Ghost::ForkMove(directions);
}
