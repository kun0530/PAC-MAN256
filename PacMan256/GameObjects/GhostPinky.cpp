#include "pch.h"
#include "GhostPinky.h"
#include "Player.h"
#include "TileMap.h"

GhostPinky::GhostPinky(const std::string& name) : Ghost(name)
{
}

void GhostPinky::Init()
{
	Ghost::Init();

	speed = 300.f;
	originTextureId = "graphics/Ghost_Pink.png";
	SetTexture(originTextureId);

	color = sf::Color::Color(255, 51, 153);
}

void GhostPinky::Release()
{
	Ghost::Release();
}

void GhostPinky::Reset()
{
	Ghost::Reset();
	direction = { 0.f, 0.f };
	isMove = false;
}

void GhostPinky::Update(float dt)
{
	Ghost::Update(dt);

	if (isMove)
	{
		if (tileMap->IsBlocked(gridIndex.x + direction.x, gridIndex.y + direction.y))
		{
			direction = { 0.f, 0.f };
			nextPos = currentPos;
			isMove = false;
		}
	}
}

void GhostPinky::FixedUpdate(float dt)
{
	Ghost::FixedUpdate(dt);
}

void GhostPinky::Draw(sf::RenderWindow& window)
{
	Ghost::Draw(window);
}

void GhostPinky::DecideDirection()
{
	if (isMove || currentTileMapId != player->GetCurrentTileMapId())
		return;

	if (gridIndex.x == player->GetGridIndex().x)
	{
		if (gridIndex.y < player->GetGridIndex().y)
		{
			for (int i = gridIndex.y; i <= player->GetGridIndex().y; ++i)
			{
				if (tileMap->IsBlocked(gridIndex.x, i))
					return;
			}
			direction = { 0.f, 1.f };
			isMove = true;
		}
		else if (gridIndex.y > player->GetGridIndex().y)
		{
			for (int i = player->GetGridIndex().y; i <= gridIndex.y; ++i)
			{
				if (tileMap->IsBlocked(gridIndex.x, i))
					return;
			} 
			direction = { 0.f, -1.f };
			isMove = true;
		}
	}
	else if (gridIndex.y == player->GetGridIndex().y)
	{
		if (gridIndex.x < player->GetGridIndex().x)
		{
			for (int i = gridIndex.x; i <= player->GetGridIndex().x; ++i)
			{
				if (tileMap->IsBlocked(i, gridIndex.y))
					return;
			}
			direction = { 1.f, 0.f };
			isMove = true;
		}
		else if (gridIndex.x > player->GetGridIndex().x)
		{
			for (int i = player->GetGridIndex().x; i <= gridIndex.x; ++i)
			{
				if (tileMap->IsBlocked(i, gridIndex.y))
					return;
			}
			direction = { -1.f, 0.f };
			isMove = true;
		}
	}
}

void GhostPinky::CornerMove(std::vector<sf::Vector2f>& directions)
{
}

void GhostPinky::ForkMove(std::vector<sf::Vector2f>& directions)
{
}
