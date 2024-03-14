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
	
	// gridIndex ·£´ý ¼³Á¤ÇÏ°í
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

	// ´ÙÀ½ ¼¿¿¡ µµÂøÇßÀ» ¶§
	if (timer > moveTime)
	{
		timer = 0.f;
		gridIndex.x += (int)direction.x;
		gridIndex.y += (int)direction.y;
		currentPos = nextPos;
		SetPosition(currentPos);

		std::vector<sf::Vector2f> directions;
		int pathCount = sceneGame->CountPathNum(gridIndex.x, gridIndex.y, tileMap, directions);

		if (pathCount == 1)
		{
			direction = directions[0];
		}
		else if (pathCount == 2)
		{
			if (direction == -directions[0])
				direction = directions[1];
			else
				direction = directions[0];
		}
		else if (pathCount >= 3)
		{
			sf::Vector2f nextDirection;
			do
			{
				nextDirection = directions[Utils::RandomRange(0, directions.size())];
			} while (nextDirection == -direction);
			direction = nextDirection;
		}

		if (gridIndex.y + (int)direction.y < 0)
		{
			tileMap = sceneGame->GetNextTileMap();
			++currentTileMapId;
			gridIndex.y = tileMap->GetCellCount().y;
		}
		/*else if (gridIndex.y + (int)direction.y >= tileMap->GetCellCount().y)
		{

		}*/

		nextPos = tileMap->GetGridPosition(gridIndex.x + (int)direction.x , gridIndex.y + (int)direction.y);
		moveTime = Utils::Magnitude(nextPos - currentPos) / speed;
	}
	
	timer += dt;
	Translate(direction * speed * dt);

	if (gridIndex == player->GetGridIndex())
	{
		if (player->GetUsingItem() == ItemType::POWER_COOKIE)
			OnDie();
		else
			player->OnDie();
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

void Ghost::ChangeMode()
{
	if (player->GetUsingItem() == ItemType::POWER_COOKIE)
		SetTexture("graphics/Ghost_Glitch.png");
	else
		SetTexture("graphics/Ghost_Red.png");
}

void Ghost::OnDie()
{
	SetActive(false);
}
