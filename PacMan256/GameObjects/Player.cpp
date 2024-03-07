#include "pch.h"
#include "Player.h"
#include "TileMap.h"

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

	/*if (timer > moveTime)
	{
		
	}

	if (moveState != MoveState::STOP)
		Translate(direction * speed * dt);*/

	/*if (InputMgr::GetKeyDown(sf::Keyboard::Up) &&
		!tileMap->IsBlocked(playerGridIndex.x, playerGridIndex.y - 1))
	{
		moveState = MoveState::UP;
		nextPos = tileMap->GetGridPosition(playerGridIndex.x, playerGridIndex.y - 1);
		direction = { 0.f, -1.f };
		timer = 0;
	}
	else if (InputMgr::GetKeyDown(sf::Keyboard::Down) &&
		!tileMap->IsBlocked(playerGridIndex.x, playerGridIndex.y + 1))
	{
		moveState = MoveState::DOWN;
		nextPos = tileMap->GetGridPosition(playerGridIndex.x, playerGridIndex.y + 1);
		direction = { 0.f, 1.f };
		timer = 0;
	}
	else if (InputMgr::GetKeyDown(sf::Keyboard::Right) &&
		!tileMap->IsBlocked(playerGridIndex.x + 1, playerGridIndex.y))
	{
		moveState = MoveState::RIGHT;
		nextPos = tileMap->GetGridPosition(playerGridIndex.x + 1, playerGridIndex.y);
		direction = { 1.f, 0.f };
		timer = 0;
	}
	else if (InputMgr::GetKeyDown(sf::Keyboard::Left) &&
		!tileMap->IsBlocked(playerGridIndex.x - 1, playerGridIndex.y))
	{
		moveState = MoveState::LEFT;
		nextPos = tileMap->GetGridPosition(playerGridIndex.x - 1, playerGridIndex.y);
		direction = { -1.f, 0.f };
		timer = 0;
	}
	moveTime = Utils::Magnitude(nextPos - currentPos) / speed;*/

	
	/*if (timer > moveTime)
	{
		currentPos = nextPos;
		SetPosition(currentPos);

		switch (moveState)
		{
		case MoveState::UP:
			playerGridIndex.y--;
			break;
		case MoveState::DOWN:
			playerGridIndex.y++;
			break;
		case MoveState::RIGHT:
			playerGridIndex.x++;
			break;
		case MoveState::LEFT:
			playerGridIndex.x--;
			break;
		}

		
		if (tileMap->IsBlocked(playerGridIndex.x, playerGridIndex.y))
		{
			moveState = MoveState::STOP;
		}
		else
		{
			nextPos = tileMap->GetGridPosition(playerGridIndex.x, playerGridIndex.y);
		}
		timer = 0;
	}
	else if (moveState != MoveState::STOP)
	{
		timer += dt;
		Translate(direction * speed * dt);
	}*/
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
