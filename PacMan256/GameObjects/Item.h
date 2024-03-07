#pragma once
#include "SpriteGo.h"
class Item : public SpriteGo
{
protected:

public:
	Item(const std::string& name = "");
	~Item() override = default;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void FixedUpdate(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

