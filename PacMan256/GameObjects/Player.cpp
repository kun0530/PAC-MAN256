#include "pch.h"
#include "Player.h"
#include "TileMap.h"
#include "SceneGame.h"
#include "Ghost.h"
#include "Item.h"

Player::Player(const std::string& name) : SpriteGo(name)
{
}

void Player::Init()
{
	SpriteGo::Init();

	SetTexture("graphics/Pacman_1.png");
	SetOrigin(Origins::MC);
	// SetPosition({ 0.f, 0.f });
	SetScale({ 0.5f, 0.5f });

	animator.SetTarget(&sprite);
}

void Player::Release()
{
	SpriteGo::Release();
}

void Player::Reset()
{
	SpriteGo::Reset();

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	currentTileMap = sceneGame->GetCurrentTileMap();

	gridIndex = { 13, 16 };
	isArrive = true;
	isWarp = false;
	currentPos = currentTileMap->GetGridPosition(gridIndex.x, gridIndex.y);
	SetPosition(currentPos);

	timer = 0.f;

	animator.Play("animations/PacManMove.csv");
}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);
	animator.Update(dt);

	sf::Vector2i inputDirection = InputMgr::GetAxisOne();
	if (inputDirection != sf::Vector2i(0, 0))
		inputDirections.push(inputDirection);

	if (isArrive)
	{
		bool isMove = false;

		while (!inputDirections.empty())
		{
			sf::Vector2i dir = inputDirections.front();
			inputDirections.pop();

			/*int newIndexX = gridIndex.x + dir.x;
			int newIndexY = gridIndex.y + dir.y;*/

			if ((!currentTileMap->IsBlocked(gridIndex.x + dir.x, gridIndex.y + dir.y)) ||
				((gridIndex.y + dir.y >= currentTileMap->GetCellCount().y)
					&& (!sceneGame->GetPrevTileMap()->IsBlocked(gridIndex.x, 0))))
			{ 
				direction = (sf::Vector2f)dir;
				isMove = true;
			}
		}

		inputDirections.push((sf::Vector2i)direction);

		if (isMove)
		{
			sf::Vector2i nextGridIndex = { gridIndex.x + (int)direction.x, gridIndex.y + (int)direction.y };
			if (nextGridIndex.y < 0)
			{
				currentTileMap = sceneGame->ChangeTileMap(true);
				nextPos = currentTileMap->GetGridPosition(gridIndex.x, currentTileMap->GetCellCount().y - 1);
				gridIndex.y = currentTileMap->GetCellCount().y;
			}
			else if (nextGridIndex.y >= currentTileMap->GetCellCount().y)
			{
				currentTileMap = sceneGame->ChangeTileMap(false);
				nextPos = currentTileMap->GetGridPosition(gridIndex.x, 0);
				gridIndex.y = -1;
			}
			else if (nextGridIndex.x < 0)
			{
				nextPos = currentPos - sf::Vector2f(currentTileMap->GetCellSize().x, 0.f);
				gridIndex.x = currentTileMap->GetCellCount().x;
				isWarp = true;
			}
			else if (nextGridIndex.x >= currentTileMap->GetCellCount().x)
			{
				nextPos = currentPos + sf::Vector2f(currentTileMap->GetCellSize().x, 0.f);
				gridIndex.x = -1;
				isWarp = true;
			}
			else
			{
				nextPos = currentTileMap->GetGridPosition(nextGridIndex.x, nextGridIndex.y);
			}

			moveTime = Utils::Magnitude(nextPos - currentPos) / speed;
			isArrive = false;
			timer = 0.f;

			SetRotation(Utils::Angle(direction));
		}
	}
	else
	{
		if (timer > moveTime)
		{
			if (isWarp)
			{
				nextPos = currentTileMap->GetGridPosition(gridIndex.x + (int)direction.x, gridIndex.y + (int)direction.y);
				currentPos = nextPos - (currentTileMap->GetCellSize().x * direction);
				SetPosition(currentPos);
				moveTime = Utils::Magnitude(nextPos - currentPos) / speed;
				timer = 0.f;
				isWarp = false;
			}
			else
			{
				currentPos = nextPos;
				SetPosition(currentPos);

				gridIndex.x += (int)direction.x;
				gridIndex.y += (int)direction.y;

				EatItem();

				isArrive = true;
			}
		}
		else
		{
			timer += dt;
			Translate(direction * speed * dt);
		}
	}

	// 애니메이션
	if (isArrive)
		animator.Stop();
	else if (!animator.IsPlaying())
		animator.Play("animations/PacManMove.csv");
	

	// 아이템
	if (usingItem != ItemType::NONE)
	{
		itemTimer += dt;
		if (itemTimer > itemDuration)
		{
			EndUsingItem();
			// SetUsingItem(ItemType::NONE);
		}
	}
}

void Player::FixedUpdate(float dt)
{
	SpriteGo::FixedUpdate(dt);
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Player::EatItem()
{
	auto itemInfo = currentTileMap->GetItem(gridIndex);
	ItemType itemType = itemInfo->GetItemType();

	if (itemInfo == nullptr)
		return;

	switch (itemType)
	{
	case ItemType::NONE:
		sceneGame->ResetChain();
		return;
	case ItemType::COOKIE:
		sceneGame->AddScore(1);
		sceneGame->AddChain();
		break;
	case ItemType::FRUIT:
		sceneGame->SetScoreMultiplier(itemInfo->GetValue());
		break;
	case ItemType::POWER_COOKIE:
		SetUsingItem(itemType);
		break;
	}
	itemInfo->SetActive(false);
	itemInfo->SetItemType(ItemType::NONE);
}

void Player::SetFruitItem(int value)
{
}

void Player::SetUsingItem(ItemType item)
{
	ItemType prevUsingItem = usingItem;
	usingItem = item;

	if (prevUsingItem != ItemType::NONE && prevUsingItem != ItemType::COOKIE)
	{
		itemDuration += 1.f;
		return;
	}

	switch (usingItem)
	{
	case ItemType::POWER_COOKIE:
		itemDuration = 5.f;
		break;
	default:
		break;
	}

	sceneGame->ChangeGhostMode();
	itemTimer = 0.f;
}

void Player::EndUsingItem()
{
	switch (usingItem)
	{
	case ItemType::POWER_COOKIE:
		break;
	default:
		break;
	}

	itemTimer = 0.f;
	itemDuration = 0.f;
	usingItem = ItemType::NONE;
	sceneGame->ChangeGhostMode();
}

void Player::OnDie()
{
	// Game Over!!!
	/*FRAMEWORK.SetTimeScale(0.f);
	SetActive(false);*/
}