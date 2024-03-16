#include "pch.h"
#include "DeathEffectGo.h"
#include "SceneGame.h"

DeathEffectGo::DeathEffectGo(const std::string& name) : GameObject(name)
{
}

void DeathEffectGo::Init()
{
	GameObject::Init();

	for (int i = 0; i < 50; ++i)
	{
		sf::RectangleShape* rect = new sf::RectangleShape();
		Utils::SetOrigin(*rect, Origins::MC);
		rect->setFillColor(effectColor);
		rect->setPosition(initPosition);
		float randSize = Utils::RandomRange(3.f, 15.f);
		rect->setSize({ randSize , randSize });
		float randAngle = Utils::RandomRange(0.f, 360.f);
		rect->setRotation(randAngle);
		sf::Vector2f randDirection = Utils::RandomOnUnitCircle();

		std::pair<sf::RectangleShape*, sf::Vector2f> rectPair;
		rectPair.first = rect;
		rectPair.second = randDirection;
		rects.push_back(rectPair);
	}
}

void DeathEffectGo::Release()
{
	GameObject::Release();
}

void DeathEffectGo::Reset()
{
	GameObject::Reset();

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
}

void DeathEffectGo::Update(float dt)
{
	GameObject::Update(dt);

	if (effectTimer > effectDuration)
	{
		for (auto rect : rects)
		{
			if (rect.first != nullptr)
			{
				delete rect.first;
				rect.first = nullptr;
			}
		}
		rects.clear();

		sceneGame->RemoveGo(this);
	}
	else
	{
		effectTimer += dt;

		for (auto rect : rects)
		{
			if (rect.first != nullptr)
			{
				sf::Vector2f rectPos = rect.first->getPosition();
				rect.first->setPosition(rectPos + rect.second * rectSpeed * dt);
			}
		}
	}
}

void DeathEffectGo::FixedUpdate(float dt)
{
	GameObject::FixedUpdate(dt);
}

void DeathEffectGo::Draw(sf::RenderWindow& window)
{
	GameObject::Draw(window);

	for (auto rect : rects)
	{
		if (rect.first != nullptr)
			window.draw(*rect.first);
	}
}
