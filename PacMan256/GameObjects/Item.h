#pragma once
#include "SpriteGo.h"
class Item : public SpriteGo
{
protected:
	sf::Vector2i gridIndex = { 0, 0 };

public:
	Item(const std::string& name = "");
	~Item() override = default;

	void SetGridIndex(const int x, const int y) { gridIndex = { x, y }; }

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void FixedUpdate(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

