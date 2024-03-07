#include "pch.h"
#include "Ghost.h"
#include "TileMap.h"

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

	tileMap = dynamic_cast<TileMap*>(SCENE_MGR.GetCurrentScene()->FindGo("Background"));
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
		currentPos = nextPos;
		SetPosition(currentPos);

		std::vector<sf::Vector2f> directions;

		if (tileMap->IsCorner(gridIndex.x, gridIndex.y))
		{
			if (direction.x != 0.f)
				direction = { 0.f, 1.f };
			else
				direction = { 1.f, 0.f };

			if (tileMap->IsBlocked(gridIndex.x + (int)direction.x, gridIndex.y + (int)direction.y))
			{
				direction *= -1.f;
			}
		}
		else if (tileMap->IsFork(gridIndex.x, gridIndex.y, directions))
		{
			sf::Vector2f nextDirection;
			do
			{
				nextDirection = directions[Utils::RandomRange(0, directions.size())];
			} while (nextDirection == -direction);
			direction = nextDirection;
		}

		gridIndex.x += (int)direction.x;
		gridIndex.y += (int)direction.y;
		nextPos = tileMap->GetGridPosition(gridIndex.x, gridIndex.y);
		moveTime = Utils::Magnitude(nextPos - currentPos) / speed;
	}
	
	timer += dt;
	Translate(direction * speed * dt);
}

void Ghost::FixedUpdate(float dt)
{
	SpriteGo::FixedUpdate(dt);
}

void Ghost::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
