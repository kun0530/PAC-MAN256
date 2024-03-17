#include "pch.h"
#include "SceneTitle.h"
#include "SpriteGo.h"
#include "TextGo.h"

SceneTitle::SceneTitle(SceneIds id) : Scene(id)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	title.setFillColor(sf::Color::Color(34, 213, 226));
	Utils::SetOrigin(title, Origins::MC);
	title.setPosition({ 0.f, 0.f });
	title.setSize((sf::Vector2f)FRAMEWORK.GetWindowSize());

	logo = new SpriteGo("Logo");
	logo->SetTexture("graphics/Main_Logo_Final.png");
	logo->SetOrigin(Origins::TC);
	// logo->SetScale({ 0.5f, 0.5f });
	logo->SetPosition({0.f, -worldView.getSize().y / 2.5f});
	AddGo(logo);

	textMessage = new TextGo("Message");
	textMessage->Set(RES_MGR_FONT.Get("fonts/editundo.ttf"),
		"Press Enter to Start!", 50, sf::Color::White);
	textMessage->SetOrigin(Origins::TC);
	textMessage->SetPosition({ 0.f, 100.f });
	AddGo(textMessage);

	for (int i = 0; i < themeCount; ++i)
	{
		SpriteGo* theme = new SpriteGo("Theme");
		std::string themeId = "graphics/Theme" + std::to_string(i) + ".png";
		theme->SetTexture(themeId);
		theme->SetOrigin(Origins::MC);
		theme->SetPosition({ 1920.f , 0.f });
		theme->sortLayer = -2;
		themes.push_back(theme);
		AddGo(theme);
	}

	Scene::Init();
}

void SceneTitle::Release()
{
	Scene::Release();
}

void SceneTitle::Enter()
{
	FRAMEWORK.SetTimeScale(1.f);

	SOUND_MGR.PlaySfx("sounds/GEN_LEVEL_BEGIN.wav");

	isMovingImage = false;
	imageDirection = { 0.f, 0.f };
	for (auto theme : themes)
	{
		theme->SetPosition({ 1920.f , 0.f });
		theme->sortLayer = -2;
	}

	Scene::Enter();
}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::Update(float dt)
{
	Scene::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		SCENE_MGR.ChangeScene(SceneIds::SCENE_GAME);
		SOUND_MGR.StopAll();
		SOUND_MGR.PlaySfx("sounds/UI_PLAY_CLICK.wav");
	}

	int themeNum = SCENE_MGR.GetThemeNum();
	if (isMovingImage)
	{
		if (imageTimer > imageMoveTime)
		{
			imageTimer = 0.f;
			isMovingImage = false;
			themes[themeNum]->sortLayer = -2;
			themes[prevThemeNum]->SetPosition({ 1920.f , 0.f });
		}
		else
		{
			imageTimer += dt;
			themes[themeNum]->Translate(imageDirection * imageSpeed * dt);
		}
	}
	else
	{
		if (InputMgr::GetKeyDown(sf::Keyboard::Right) && themeNum < themeCount - 1)
		{
			// themes[themeNum]->sortLayer = -2;
			prevThemeNum = themeNum;
			themes[++themeNum]->sortLayer = -1;
			ResortGo(themes[themeNum]);
			themes[themeNum]->SetPosition({ 1920.f , 0.f });
			imageMoveTime = 1920.f / imageSpeed;
			imageDirection = { -1.f, 0.f };
			isMovingImage = true;
		}
		else if (InputMgr::GetKeyDown(sf::Keyboard::Left) && themeNum > 0)
		{
			// themes[themeNum]->sortLayer = -2;
			prevThemeNum = themeNum;
			themes[--themeNum]->sortLayer = -1;
			ResortGo(themes[themeNum]);
			themes[themeNum]->SetPosition({ -1920.f , 0.f });
			imageMoveTime = 1920.f / imageSpeed;
			imageDirection = { 1.f, 0.f };
			isMovingImage = true;
		}
		SCENE_MGR.SetThemeNum(themeNum);
	}
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	window.draw(title);

	Scene::Draw(window);
}
