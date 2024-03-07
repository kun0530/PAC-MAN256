#pragma once
#include "Item.h"
class PowerCookieItem : public Item
{
protected:

public:
	PowerCookieItem(const std::string& name = "");
	~PowerCookieItem() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

