#include "pch.h"
#include "GhostBlinky.h"
#include "Player.h"

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
	sf::Vector2f targetDir = { 0.f, 0.f };
	
	int mapIdDiff = currentTileMapId - player->GetCurrentTileMapId();
	int indexDiffY = player->GetGridIndex().y - gridIndex.y;
	int indexDiffX = player->GetGridIndex().x - gridIndex.x;

	if (mapIdDiff == 0)
	{
		if (indexDiffY != 0)
			targetDir.y = indexDiffY / abs(indexDiffY);
	}
	else
		targetDir.y = mapIdDiff / abs(mapIdDiff);

	if (indexDiffX != 0)
		targetDir.x = indexDiffX / abs(indexDiffX);

	std::cout << targetDir.x << ", " << targetDir.y << std::endl;

	for (auto dir : directions)
	{
		// 나중에 구현
	}
}
