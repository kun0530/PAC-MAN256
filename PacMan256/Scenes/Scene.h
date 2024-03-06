#pragma once

class GameObject;

class Scene
{
public:
	enum Layers
	{
		None = 0,
		World = 1,
		Ui = 2,
		Everything = 0xFFFFFFFF // 비트 연산자 &을 사용하면 동일한 값이 나온다.
	};

protected:
	SceneIds id;

	std::list<GameObject*> gameObjects;
	std::list<GameObject*> uiGameObjects;

	std::list<GameObject*> removeGameObjects;
	std::list<GameObject*> resortingGameObjects;

	sf::View worldView;
	sf::View uiView;

	ResourceMgr<sf::Texture>& texResMgr;
	ResourceMgr<sf::Font>& fontResMgr;
	ResourceMgr<sf::SoundBuffer>& soundResMgr;
public:
	Scene(SceneIds id);
	virtual ~Scene() = default;

	sf::Vector2f ScreenToWorld(sf::Vector2i screenPos);
	sf::Vector2i WorldToScreen(sf::Vector2f worldPos);
	sf::Vector2f ScreenToUi(sf::Vector2i screenPos);
	sf::Vector2i UiToScreen(sf::Vector2f uiPos);

	virtual void Init();
	virtual void Release();

	virtual void Enter();
	virtual void Exit() {};

	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void FixedUpdate(float dt);
	virtual void Draw(sf::RenderWindow& window);

	virtual GameObject* FindGo(const std::string& name, Layers layer = Layers::Everything);
	virtual int FindGoAll(const std::string& name, std::list<GameObject*>& list,
		Layers layer = Layers::Everything);

	virtual GameObject* AddGo(GameObject* obj, Layers layer = Layers::World);
	virtual void RemoveGo(GameObject* obj);
	virtual void ResortGo(GameObject* obj);

	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;
	Scene& operator=(const Scene&) = delete;
	Scene& operator=(Scene&&) = delete;
};

