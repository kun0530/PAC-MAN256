#include "pch.h"
#include "Player.h"
#include "TileMap.h"
#include "SceneGame.h"
#include "Ghost.h"

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
}

void Player::Release()
{
	SpriteGo::Release();
}

void Player::Reset()
{
	SpriteGo::Reset();

	tileMap = dynamic_cast<TileMap*>(SCENE_MGR.GetCurrentScene()->FindGo("Background"));
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());

	playerGridIndex = { 13, 16 };
	MoveState moveState = MoveState::STOP;
	currentPos = tileMap->GetGridPosition(playerGridIndex.x, playerGridIndex.y);
	SetPosition(currentPos);

	timer = 0.f;
}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);

	sf::Vector2i inputDirection = InputMgr::GetAxisOne();
	IsMove(inputDirection);

	if (timer > moveTime)
	{
		timer = 0.f;
		currentPos = nextPos;
		SetPosition(currentPos);
		moveState = MoveState::STOP;
		IsMove((sf::Vector2i)direction);
	}

	if (moveState != MoveState::STOP)
	{
		timer += dt;
		Translate(direction * speed * dt);
	}

	// 아이템
	if (itemMode != ItemMode::NONE)
	{
		itemTimer += dt;
		if (itemTimer > itemDuration)
		{
			SetItemMode(ItemMode::NONE);
		}
	}
}

bool Player::IsMove(sf::Vector2i dir)
{
	if (dir.x != 0 || dir.y != 0 || moveState == MoveState::STOP)
	{
		if (!tileMap->IsBlocked(playerGridIndex.x + dir.x,
			playerGridIndex.y + dir.y))
		{
			if (dir.x > 0)
				moveState = MoveState::RIGHT;
			if (dir.x < 0)
				moveState = MoveState::LEFT;
			if (dir.y > 0)
				moveState = MoveState::DOWN;
			if (dir.y < 0)
				moveState = MoveState::UP;

			playerGridIndex.x += dir.x;
			playerGridIndex.y += dir.y;
			nextPos = tileMap->GetGridPosition(playerGridIndex.x, playerGridIndex.y);
			direction = (sf::Vector2f)dir;
			moveTime = Utils::Magnitude(nextPos - currentPos) / speed;
			timer = 0.f;

			return true;
		}
	}
	return false;
}

void Player::FixedUpdate(float dt)
{
	SpriteGo::FixedUpdate(dt);
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Player::OnDie()
{
	// Game Over!!!
	SetActive(false);
}

void Player::SetItemMode(ItemMode mode)
{
	itemMode = mode;

	// TO-DO: 나중에 데이터 테이블로 정리
	switch (itemMode)
	{
	case ItemMode::POWER_COOKIE:
		itemDuration = 5.f;
		break;
	default:
		break;
	}

	std::list<GameObject*> goList = sceneGame->GetGhostList();
	for (auto go : goList)
	{
		if (!go->GetActive())
			continue;

		Ghost* ghost = dynamic_cast<Ghost*>(go);
		if (ghost != nullptr)
		{
			ghost->ChangeMode();
		}
	}
	itemTimer = 0.f;
}