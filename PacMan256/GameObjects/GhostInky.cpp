#include "pch.h"
#include "GhostInky.h"

GhostInky::GhostInky(const std::string& name) : Ghost(name)
{
}

void GhostInky::Init()
{
	Ghost::Init();

	originTextureId = "graphics/Ghost_Blue.png";
	SetTexture(originTextureId);

	color = sf::Color::Blue;
}

void GhostInky::Release()
{
	Ghost::Release();
}

void GhostInky::Reset()
{
	Ghost::Reset();

	if (Utils::RandomRange(0, 2) == 0)
		rotateNum = 1; // 90도 회전
	else
		rotateNum = -1; // -90도 회전
}

void GhostInky::Update(float dt)
{
	Ghost::Update(dt);
}

void GhostInky::FixedUpdate(float dt)
{
	Ghost::FixedUpdate(dt);
}

void GhostInky::Draw(sf::RenderWindow& window)
{
	Ghost::Draw(window);
}

void GhostInky::CornerMove(std::vector<sf::Vector2f>& directions)
{
	Ghost::CornerMove(directions);
}

void GhostInky::ForkMove(std::vector<sf::Vector2f>& directions)
{
	// (-y, x): 90도 회전
	// (y, -x): -90도 회전

	sf::Vector2f rotationDirection = { direction.y * (-rotateNum), direction.x * rotateNum};

	for (auto dir : directions)
	{
		if (dir == rotationDirection) // (-)90도 회전된 방향이 있으면 방향 전환
		{
			direction = dir;
		}
	}

	if (direction == sf::Vector2f(0.f, 0.f))
		direction = directions[Utils::RandomRange(0, directions.size())];
}
