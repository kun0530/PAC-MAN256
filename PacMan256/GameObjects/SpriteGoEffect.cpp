#include "pch.h"
#include "SpriteGoEffect.h"

SpriteGoEffect::SpriteGoEffect(const std::string& name) : SpriteGo(name)
{
}

void SpriteGoEffect::Reset()
{
	timer = 0.f;
}

void SpriteGoEffect::Update(float dt)
{
	timer += dt;
	if (timer > duration)
	{
		SetActive(false);
		SCENE_MGR.GetCurrentScene()->RemoveGo(this);
	}
}
