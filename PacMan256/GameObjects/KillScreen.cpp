#include "pch.h"
#include "KillScreen.h"
#include "SceneGame.h"
#include "TileMap.h"

KillScreen::KillScreen(const std::string& name) : SpriteGo(name)
{
}

void KillScreen::Init()
{
	SpriteGo::Init();

	SetTexture("graphics/glitch-map-1.png");
}

void KillScreen::Release()
{
	SpriteGo::Release();
}

void KillScreen::Reset()
{
	SpriteGo::Reset();

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	tileMapId = sceneGame->GetCurrentTileMapId();
	if (tileMapId == 0)
		tileMap = sceneGame->GetCurrentTileMap();
	else
	{
		tileMap = sceneGame->GetPrevTileMap();
		--tileMapId;
	}
	SetOrigin(Origins::ML);
	
	gridIndexY = tileMap->GetCellCount().y - 1;
	nextGridindexY = gridIndexY - 1;

	currentPosition = tileMap->GetGridPosition(0, gridIndexY);
	nextPosition = tileMap->GetGridPosition(0, nextGridindexY);
	SetPosition(currentPosition);

	moveTime = Utils::Magnitude(nextPosition - currentPosition) / speed;
	timer = 0.f;
}

void KillScreen::Update(float dt)
{
	SpriteGo::Update(dt);

	if (sceneGame->GetCurrentTileMapId() - tileMapId >= 2)
		Reset();

	if (timer > moveTime)
	{
		currentPosition = nextPosition;
		SetPosition(currentPosition);

		gridIndexY = nextGridindexY;
		nextGridindexY = gridIndexY - 1;
		if (nextGridindexY < 0)
		{
			tileMap = sceneGame->GetCurrentTileMap();
			tileMapId = sceneGame->GetCurrentTileMapId();
			gridIndexY = tileMap->GetCellCount().y;
			nextGridindexY = gridIndexY - 1;
		}
		nextPosition = tileMap->GetGridPosition(0, nextGridindexY);

		moveTime = Utils::Magnitude(nextPosition - currentPosition) / speed;
		timer = 0.f;
	}
	else
	{
		timer += dt;
	}

	Translate(direction * speed * dt);
}

void KillScreen::FixedUpdate(float dt)
{
	SpriteGo::FixedUpdate(dt);
}

void KillScreen::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
