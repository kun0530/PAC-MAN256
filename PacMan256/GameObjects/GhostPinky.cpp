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
			nextPos = tileMap->GetGridPosition(gridIndex.x, gridIndex.y);
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
	if (isMove)
		return;

	if (gridIndex.x == player->GetGridIndex().x &&
		currentTileMapId == player->GetCurrentTileMapId())
	{
		int minY = 0;
		int maxY = 0;

		if (gridIndex.y < player->GetGridIndex().y)
		{
			minY = gridIndex.y;
			maxY = player->GetGridIndex().y;
		}
		else
		{
			minY = player->GetGridIndex().y;
			maxY = gridIndex.y;
		}

		isMove = true;
		for (int i = minY; i <= maxY; i++)
		{
			if (tileMap->IsBlocked(gridIndex.x, i))
			{
				isMove = false;
				break;
			}
		}

		if (isMove)
		{
			direction = { 0.f, (float)(player->GetGridIndex().y - gridIndex.y) / abs(player->GetGridIndex().y - gridIndex.y) };
		}
	}
	else if (gridIndex.y == player->GetGridIndex().y &&
		currentTileMapId == player->GetCurrentTileMapId())
	{
		int minX = 0;
		int maxX = 0;

		if (gridIndex.x < player->GetGridIndex().x)
		{
			minX = gridIndex.x;
			maxX = player->GetGridIndex().x;
		}
		else
		{
			minX = player->GetGridIndex().x;
			maxX = gridIndex.x;
		}

		isMove = true;
		for (int i = minX; i <= maxX; i++)
		{
			if (tileMap->IsBlocked(i, gridIndex.y))
			{
				isMove = false;
				break;
			}
		}

		if (isMove)
		{
			direction = { (float)(player->GetGridIndex().x - gridIndex.x) / abs(player->GetGridIndex().x - gridIndex.x), 0.f };
		}
	}
}

void GhostPinky::CornerMove(std::vector<sf::Vector2f>& directions)
{
}

void GhostPinky::ForkMove(std::vector<sf::Vector2f>& directions)
{
}
