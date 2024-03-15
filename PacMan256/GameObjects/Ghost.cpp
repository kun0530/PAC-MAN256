#include "pch.h"
#include "Ghost.h"
#include "SceneGame.h"
#include "TileMap.h"
#include "Player.h"

Ghost::Ghost(const std::string& name) : SpriteGo(name)
{
}

void Ghost::Init()
{
	SpriteGo::Init();

	SetTexture("graphics/Ghost_Red.png");
	SetOrigin(Origins::MC);
	SetScale({ 2.f, 2.f });
}

void Ghost::Release()
{
	SpriteGo::Release();
}

void Ghost::Reset()
{
	SpriteGo::Reset();

	// Test
	// SetActive(false);

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	tileMap = sceneGame->GetCurrentTileMap();
	player = dynamic_cast<Player*>(sceneGame->FindGo("Player"));
	currentTileMapId = sceneGame->GetCurrentTileMapId();
	
	// gridIndex 랜덤 설정하고
	SetPosition(tileMap->GetGridPosition(gridIndex.x, gridIndex.y));
	gridIndex.x += (int)direction.x;
	gridIndex.y += (int)direction.y;
	nextPos = tileMap->GetGridPosition(gridIndex.x, gridIndex.y);
	currentPos = position;
	moveTime = Utils::Magnitude(nextPos - currentPos) / speed;
	timer = 0.f;
}

void Ghost::Update(float dt)
{
	SpriteGo::Update(dt);

	if (tileMap->GetMapStatus() == MapStatus::NONE)
	{
		sceneGame->RemoveGo(this);
		return;
	}

	// 다음 셀에 도착했을 때
	if (timer > moveTime)
	{
		if (isWarp)
		{
			nextPos = tileMap->GetGridPosition(gridIndex.x + (int)direction.x, gridIndex.y + (int)direction.y);
			currentPos = nextPos - (tileMap->GetCellSize().x * direction);
			SetPosition(currentPos);
			moveTime = Utils::Magnitude(nextPos - currentPos) / speed;
			timer = 0.f;
			isWarp = false;
		}
		else
		{
			timer = 0.f;
			gridIndex.x += (int)direction.x;
			gridIndex.y += (int)direction.y;
			currentPos = nextPos;
			SetPosition(currentPos);

			DecideDirection();

			CheckTileMapBoundary();

			moveTime = Utils::Magnitude(nextPos - currentPos) / speed;
		}
	}
	
	timer += dt;
	Translate(direction * speed * dt);

	if (gridIndex == player->GetGridIndex())
	{
		if (player->GetUsingItem() == ItemType::POWER_COOKIE)
			OnDie();
		/*else
			player->OnDie();*/
	}
}

void Ghost::FixedUpdate(float dt)
{
	SpriteGo::FixedUpdate(dt);
}

void Ghost::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Ghost::DecideDirection()
{
	std::vector<sf::Vector2f> directions;
	int pathCount = sceneGame->CountPathNum(gridIndex.x, gridIndex.y, tileMap, directions);

	if (pathCount == 1)
	{
		direction = directions[0];
	}
	else if (pathCount == 2)
	{
		CornerMove(directions);
	}
	else if (pathCount >= 3)
	{
		ForkMove(directions);
	}
}

void Ghost::CheckTileMapBoundary()
{
	if (gridIndex.y + (int)direction.y < 0)
	{
		switch (tileMap->GetMapStatus())
		{
		case MapStatus::NEXT:
			direction *= -1.f;
			nextPos = tileMap->GetGridPosition(gridIndex.x + (int)direction.x, gridIndex.y + (int)direction.y);
			break;
		case MapStatus::PREV:
			tileMap = sceneGame->GetCurrentTileMap();
			++currentTileMapId;
			gridIndex.y = tileMap->GetCellCount().y;
			nextPos = tileMap->GetGridPosition(gridIndex.x, tileMap->GetCellCount().y - 1);
			break;
		case MapStatus::CURRENT:
			tileMap = sceneGame->GetNextTileMap();
			++currentTileMapId;
			gridIndex.y = tileMap->GetCellCount().y;
			nextPos = tileMap->GetGridPosition(gridIndex.x, tileMap->GetCellCount().y - 1);
			break;
		}
	}
	else if (gridIndex.y + (int)direction.y >= tileMap->GetCellCount().y)
	{
		switch (tileMap->GetMapStatus())
		{
		case MapStatus::PREV:
			direction *= -1.f;
			nextPos = tileMap->GetGridPosition(gridIndex.x + (int)direction.x, gridIndex.y + (int)direction.y);
			break;
		case MapStatus::NEXT:
			tileMap = sceneGame->GetCurrentTileMap();
			--currentTileMapId;
			gridIndex.y = -1;
			nextPos = tileMap->GetGridPosition(gridIndex.x, 0);
			break;
		case MapStatus::CURRENT:
			tileMap = sceneGame->GetPrevTileMap();
			--currentTileMapId;
			gridIndex.y = -1;
			nextPos = tileMap->GetGridPosition(gridIndex.x, 0);
			break;
		}
	}
	else if (gridIndex.x + (int)direction.x < 0)
	{
		nextPos = currentPos - sf::Vector2f(tileMap->GetCellSize().x, 0.f);
		gridIndex.x = tileMap->GetCellCount().x;
		isWarp = true;
	}
	else if (gridIndex.x + (int)direction.x >= tileMap->GetCellCount().x)
	{
		nextPos = currentPos + sf::Vector2f(tileMap->GetCellSize().x, 0.f);
		gridIndex.x = -1;
		isWarp = true;
	}
	else
	{
		nextPos = tileMap->GetGridPosition(gridIndex.x + (int)direction.x, gridIndex.y + (int)direction.y);
	}
}

void Ghost::CornerMove(std::vector<sf::Vector2f>& directions)
{
	if (direction == -directions[0])
		direction = directions[1];
	else
		direction = directions[0];
}

void Ghost::ForkMove(std::vector<sf::Vector2f>& directions)
{
	sf::Vector2f nextDirection;
	do
	{
		nextDirection = directions[Utils::RandomRange(0, directions.size())];
	} while (nextDirection == -direction);
	direction = nextDirection;
}

// 동일한 타일맵 내에서 startIndex로부터 targetIndex까지의 최단 경로 탐색
sf::Vector2i Ghost::BFS(const sf::Vector2i& startIndex, const sf::Vector2i& targetIndex, std::vector<sf::Vector2f>& directions, int tileMapDiff)
{
	sf::Vector2i dirs[4] = {
		{ 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }
	};

	int countX = tileMap->GetCellCount().x;
	int countY = tileMap->GetCellCount().y;
	
	sf::Vector2i shortestDirection;
	int shortestDistance = 5000;
	int tempDistance = 0;

	bool* visited = new bool[countX * countY];
	//bool visited[3000];

	for (auto dir : directions)
	{
		if (dir == -direction)
			continue;

		sf::Vector2i nextIndex = { startIndex.x + (int)dir.x, startIndex.y + (int)dir.y };

		if (nextIndex.x < 0 || nextIndex.y < 0 || nextIndex.x >= countX || nextIndex.y >= countY)
			continue;

		for (int i = 0; i < countX * countY; ++i)
		{
			visited[i] = false;
		}
		std::queue<std::pair<sf::Vector2i, int>> nodes;
		nodes.push(std::pair<sf::Vector2i, int>(nextIndex, 1));
		visited[startIndex.y * countX + startIndex.x] = true;
		visited[nextIndex.y * countX + nextIndex.x] = true;

		while (!nodes.empty())
		{
			sf::Vector2i index = nodes.front().first;
			int distance = nodes.front().second;
			nodes.pop();

			if ((tileMapDiff == 0 && index == targetIndex) ||
				(tileMapDiff > 0 && index.y == 0))
			{
				tempDistance = distance;
				break;
			}

			if (tileMapDiff < 0 && gridIndex.y == tileMap->GetCellCount().y - 1 && !sceneGame->GetPrevTileMap()->IsBlocked(index.x, 0))
			{
				tempDistance = distance;
				break;
			}

			for (auto dir : dirs)
			{
				sf::Vector2i newIndex = index + dir;

				if (newIndex.x < 0 || newIndex.y < 0 || newIndex.x >= countX || newIndex.y >= countY)
					continue;

				if (!tileMap->IsBlocked(newIndex.x, newIndex.y) && !visited[newIndex.y * countX + newIndex.x])
				{
					nodes.push(std::pair<sf::Vector2i, int>(newIndex, ++distance));
					visited[newIndex.y * countX + newIndex.x] = true;
				}
			}
		}
		if (shortestDistance > tempDistance)
		{
			shortestDistance = tempDistance;
			shortestDirection = (sf::Vector2i)dir;
		}
	}

	if (visited != nullptr)
	{
		delete[] visited;
		visited = nullptr;
	}

	return shortestDirection;
}

void Ghost::ChangeMode()
{
	if (player->GetUsingItem() == ItemType::POWER_COOKIE)
		SetTexture("graphics/Ghost_Glitch.png");
	else
		SetTexture(originTextureId);
}

void Ghost::OnDie()
{
	player->AddItemDuration(0.5f);
	sceneGame->AddScore(10);
	SetActive(false);
}
