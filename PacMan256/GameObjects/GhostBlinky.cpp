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
	if (currentTileMapId == player->GetCurrentTileMapId())
		direction = (sf::Vector2f)BFS(gridIndex, player->GetGridIndex(), directions);
	else if (currentTileMapId < player->GetCurrentTileMapId()) // ���� Ž��
	{
		// gridIndex.y == 0�� �ε����� ���� �ִ� ��� Ž��
		direction = (sf::Vector2f)BFS(gridIndex, player->GetGridIndex(), directions, 1);
	}
	else if (currentTileMapId > player->GetCurrentTileMapId()) // �Ʒ��� Ž��
	{
		if (gridIndex.y == tileMap->GetCellCount().y - 1 && direction != sf::Vector2f(0.f, -1.f))
		{
			for (auto dir : directions)
			{
				if (dir == sf::Vector2f(0.f, 1.f))
				{
					direction = { 0.f, 1.f };
					return;
				}
			}
		}
		// gridIndex.y == tileMap->GetCellCount().y - 1 && sceneGame->CountPathNum() >= 3�� �ε����� ���� �ִ� ��� Ž��
		direction = (sf::Vector2f)BFS(gridIndex, player->GetGridIndex(), directions, -1);
	}
}
