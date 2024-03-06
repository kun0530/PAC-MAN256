#include "pch.h"
#include "TextGo.h"

TextGo::TextGo(const std::string& name)
	: GameObject(name)
{
}

sf::FloatRect TextGo::GetLocalBounds()
{
	return text.getLocalBounds();
}

sf::FloatRect TextGo::GetGlobalBounds()
{
	return text.getGlobalBounds();
}

void TextGo::SetString(const std::string& str)
{
	text.setString(str);
	SetOrigin(originPreset);
}

void TextGo::Set(const sf::Font& texture,
	const std::string& str,
	int size, 
	const sf::Color& color)
{
	SetFont(texture);
	text.setString(str);
	text.setCharacterSize(size);
	text.setFillColor(color);
}

void TextGo::SetFont(const std::string& fontId)
{
	text.setFont(ResourceMgr<sf::Font>::Instance().Get(fontId));
}

void TextGo::SetFont(const sf::Font& font)
{
	text.setFont(font);
}

void TextGo::SetOutline(const sf::Color& color, float thick)
{
	text.setOutlineColor(color);
	text.setOutlineThickness(thick);
}

void TextGo::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	text.setPosition(position);
}

void TextGo::Translate(const sf::Vector2f& delta)
{
	position += delta;
	SetPosition(position);
}

void TextGo::SetRotation(float r)
{
	rotation = r;
	text.setRotation(r);
}

void TextGo::SetOrigin(Origins preset)
{
	if (preset == Origins::Custom)
	{
		preset = Origins::TL;
	}
	originPreset = preset;
	origin = Utils::SetOrigin(text, originPreset);
}

void TextGo::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	text.setOrigin(newOrigin);
}

void TextGo::SetScale(const sf::Vector2f& s)
{
	sf::Vector2f spriteScale = scale = s;
	if (isFlipX)
	{
		spriteScale.x = -scale.x;
	}
	if (isFlipY)
	{
		spriteScale.y = -scale.y;
	}
	text.setScale(spriteScale);
}

void TextGo::SetFlipX(bool filp)
{
	if (isFlipX == filp)
		return;

	isFlipX = filp;
	SetScale(scale);
}

void TextGo::SetFlipY(bool filp)
{
	if (isFlipY == filp)
		return;

	isFlipY = filp;
	SetScale(scale);
}

void TextGo::Draw(sf::RenderWindow& window)
{
	window.draw(text);
}
