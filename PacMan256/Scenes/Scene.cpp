#include "pch.h"
#include "Scene.h"

Scene::Scene(SceneIds id) :
	id(id),
	texResMgr(RES_MGR_TEXTURE),
	fontResMgr(RES_MGR_FONT),
	soundResMgr(RES_MGR_SOUND_BUFFER)
{
}

sf::Vector2f Scene::ScreenToWorld(sf::Vector2i screenPos)
{
	return FRAMEWORK.GetWindow().mapPixelToCoords(screenPos, worldView);
}

sf::Vector2i Scene::WorldToScreen(sf::Vector2f worldPos)
{
	return FRAMEWORK.GetWindow().mapCoordsToPixel(worldPos, worldView);
}

sf::Vector2f Scene::ScreenToUi(sf::Vector2i screenPos)
{
	return FRAMEWORK.GetWindow().mapPixelToCoords(screenPos, uiView);
}

sf::Vector2i Scene::UiToScreen(sf::Vector2f uiPos)
{
	return FRAMEWORK.GetWindow().mapCoordsToPixel(uiPos, uiView);
}

void Scene::Init()
{
	sf::Vector2f windowSize = (sf::Vector2f)FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;
	worldView.setSize(windowSize);
	worldView.setCenter({ 0.f, 0.f });
	uiView.setSize(windowSize);
	uiView.setCenter(centerPos);

	for (auto obj : gameObjects)
	{
		obj->Init();
	}

	for (auto obj : uiGameObjects)
	{
		obj->Init();
	}
}

void Scene::Release()
{
	for (auto obj : gameObjects)
	{
		delete obj;
	}

	for (auto obj : uiGameObjects)
	{
		delete obj;
	}

	gameObjects.clear();
	uiGameObjects.clear();
}

void Scene::Enter()
{
	for (auto obj : gameObjects)
	{
		obj->Reset();
	}

	for (auto obj : uiGameObjects)
	{
		obj->Reset();
	}
}

void Scene::Update(float dt)
{
	for (auto obj : gameObjects)
	{
		if (obj->GetActive())
		{
			obj->Update(dt);
		}
	}

	for (auto obj : uiGameObjects)
	{
		if (obj->GetActive())
		{
			obj->Update(dt);
		}
	}
}

void Scene::LateUpdate(float dt)
{
	for (auto obj : gameObjects)
	{
		if (obj->GetActive())
		{
			obj->LateUpdate(dt);
		}
	}

	for (auto obj : uiGameObjects)
	{
		if (obj->GetActive())
		{
			obj->LateUpdate(dt);
		}
	}

	for (auto obj : resortingGameObjects)
	{
		auto it = std::find(gameObjects.begin(), gameObjects.end(), obj);
		if (it != gameObjects.end())
		{
			gameObjects.remove(obj);
			AddGo(obj, Layers::World);
			continue;
		}

		it = std::find(uiGameObjects.begin(), uiGameObjects.end(), obj);
		if (it != uiGameObjects.end())
		{
			uiGameObjects.remove(obj);
			AddGo(obj, Layers::Ui);
			continue;
		}
	}

	for (auto obj : removeGameObjects)
	{
		gameObjects.remove(obj);
		uiGameObjects.remove(obj);

		delete obj;
	}
	removeGameObjects.clear();
}

void Scene::FixedUpdate(float dt)
{
	for (auto obj : gameObjects)
	{
		if (obj->GetActive())
		{
			obj->FixedUpdate(dt);
		}
	}

	for (auto obj : uiGameObjects)
	{
		if (obj->GetActive())
		{
			obj->FixedUpdate(dt);
		}
	}
}

void Scene::Draw(sf::RenderWindow& window)
{
	// gameObjects.sort();
	//std::sort(gameObjects.begin(), gameObjects.end(),
	//	[](GameObject* a, GameObject* b) {
	//		// a + b
	//		if (a->sortLayer != b->sortLayer)
	//		{
	//			return a->sortLayer < b->sortLayer;
	//		}
	//		return a->sortOrder < b->sortOrder;
	//	});
	//std::sort(uiGameObjects.begin(), uiGameObjects.end(),
	//	[](GameObject* a, GameObject* b) {
	//		// a + b
	//		if (a->sortLayer != b->sortLayer)
	//		{
	//			return a->sortLayer < b->sortLayer;
	//		}
	//		return a->sortOrder < b->sortOrder;
	//	});

	const sf::View& saveView = window.getView();

	window.setView(worldView);

	for (auto obj : gameObjects)
	{
		if (obj->GetActive())
		{
			obj->Draw(window);
		}
	}

	window.setView(uiView);
	for (auto obj : uiGameObjects)
	{
		if (obj->GetActive())
		{
			obj->Draw(window);
		}
	}

	window.setView(saveView); // 처음 사용한 뷰를 다시 되돌린다.
}

GameObject* Scene::FindGo(const std::string& name, Layers layer)
{
	if ((layer & Layers::World) == Layers::World)
	{
		for (auto obj : gameObjects)
		{
			if (obj->name == name)
			{
				return obj;
			}
		}
	}

	if ((layer & Layers::Ui) == Layers::Ui)
	{
		for (auto obj : uiGameObjects)
		{
			if (obj->name == name)
			{
				return obj;
			}
		}
	}

	return nullptr;
}

int Scene::FindGoAll(const std::string& name, std::list<GameObject*>& list, Layers layer)
{
	list.clear();

	if ((layer & Layers::World) == Layers::World)
	{
		for (auto obj : gameObjects)
		{
			if (obj->name == name)
			{
				list.push_back(obj);
			}
		}
	}

	if ((layer & Layers::Ui) == Layers::Ui)
	{
		for (auto obj : uiGameObjects)
		{
			if (obj->name == name)
			{
				list.push_back(obj);
			}
		}
	}

	return list.size();
}

GameObject* Scene::AddGo(GameObject* obj, Layers layer)
{
	if (layer == Layers::World)
	{
		if (std::find(gameObjects.begin(), gameObjects.end(), obj) == gameObjects.end())
		{
			if (gameObjects.empty())
			{
				gameObjects.push_back(obj);
				return obj;
			}

			auto it = gameObjects.begin();
			while (it != gameObjects.end())
			{
				if (GameObject::CompareDrawOrder(obj, *it))
				{
					gameObjects.insert(it, obj);
					return obj;
				}
				++it;
			}
			gameObjects.push_back(obj);
			return obj;
		}
	}

	if (layer == Layers::Ui)
	{
		if (std::find(uiGameObjects.begin(), uiGameObjects.end(), obj) == uiGameObjects.end())
		{
			if (uiGameObjects.empty())
			{
				uiGameObjects.push_back(obj);
				return obj;
			}

			auto it = uiGameObjects.begin();
			while (it != uiGameObjects.end())
			{
				if (GameObject::CompareDrawOrder(obj, *it))
				{
					uiGameObjects.insert(it, obj);
					return obj;
				}
				++it;
			}
			uiGameObjects.push_back(obj);
			return obj;
		}
	}
	return nullptr;
}

void Scene::RemoveGo(GameObject* obj)
{
	//obj->SetActive(false);
	if (std::find(removeGameObjects.begin(), removeGameObjects.end(), obj) == removeGameObjects.end())
	{
		removeGameObjects.push_back(obj);
	}
}

void Scene::ResortGo(GameObject* obj)
{
	if (std::find(resortingGameObjects.begin(), resortingGameObjects.end(), obj) == resortingGameObjects.end())
	{
		resortingGameObjects.push_back(obj);
	}
}
