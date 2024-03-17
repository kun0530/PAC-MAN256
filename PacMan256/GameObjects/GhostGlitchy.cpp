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

	for (int i = 0; i <= 3; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			SpriteGo* glitchMap = new SpriteGo("Glitch Map");
			glitchMap->SetTexture("graphics/glitch-map-" + std::to_string(i) + ".png");
			glitchMap->SetOrigin(Origins::MC);
			glitchParts.push_back(glitchMap);
		}
	}
}

void GhostGlitchy::Release()
{
	Ghost::Release();
}

void GhostGlitchy::Reset()
{
	Ghost::Reset();

	isMove = true;
	isWarpStart = false;
	isWarpEnd = false;

	moveTimer = 0.f;
	warpStartTimer = 0.f;
	warpEndTimer = 0.f;
}

void GhostGlitchy::Update(float dt)
{
	if (isMove)
	{
		if (moveTimer > moveDuration)
		{
			isMove = false;
			isWarpStart = true;
			warpStartTimer = 0.f;
		}
		else
		{
			moveTimer += dt;
			Ghost::Update(dt);
		}
	}
	else if (isWarpStart)
	{
		if (warpStartTimer > warpStartDuration)
		{
			isWarpStart = false;
			isWarpEnd = true;
			warpEndTimer = 0.f;

			RePosition();
		}
		else
		{
			warpStartTimer += dt;
			WarpAnimation(dt);
		}
	}
	else if (isWarpEnd)
	{
		if (warpEndTimer > warpEndDuration)
		{
			isWarpEnd = false;
			isMove = true;
			moveTimer = 0.f;
		}
		{
			warpEndTimer += dt;
			WarpAnimation(dt);
		}
	}
}

void GhostGlitchy::FixedUpdate(float dt)
{
	Ghost::FixedUpdate(dt);
}

void GhostGlitchy::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);

	if (isWarpStart || isWarpEnd)
	{
		for (auto parts : glitchParts)
		{
			parts->Draw(window);
		}
	}
}

void GhostGlitchy::CornerMove(std::vector<sf::Vector2f>& directions)
{
	Ghost::CornerMove(directions);
}

void GhostGlitchy::ForkMove(std::vector<sf::Vector2f>& directions)
{
	Ghost::ForkMove(directions);
}

void GhostGlitchy::RePosition()
{
	moveTimer = 0.f;
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

void GhostGlitchy::WarpAnimation(float dt)
{
	for (auto parts : glitchParts)
	{
		float randPosX = Utils::RandomRange(-16.f, 16.f);
		float randPosY = Utils::RandomRange(-16.f, 16.f);
		parts->SetPosition(position + sf::Vector2f(randPosX, randPosY));
	}
}