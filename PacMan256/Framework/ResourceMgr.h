#pragma once
#include "Singleton.h"

template<typename T>
class ResourceMgr : public Singleton<ResourceMgr<T>>
{
	friend Singleton<ResourceMgr<T>>;

private:
	std::unordered_map<std::string, T*> resources;

	ResourceMgr() = default;
	virtual ~ResourceMgr()
	{
		UnloadAll();
	}

public:
	static T Empty;

	bool Load(const std::string& filePath)
	{
		if (resources.find(filePath) != resources.end())
			return false;

		T* res = new T();
		bool success = res->loadFromFile(filePath);
		if (success)
		{
			//resources[id] = res;
			resources.insert({ filePath, res });
			//resources.insert(std::unordered_map<std::string, T*>::make_pair(id, res))
		}
		return success;
	}

	void UnloadAll()
	{
		for (const auto& pair : resources)
		{
			delete pair.second;
		}
		resources.clear();
	}

	bool Unload(const std::string& filePath)
	{
		auto it = resources.find(filePath);
		if (it == resources.end())
			return false;

		delete it->second;
		resources.erase(it);
		return true;
	}

	T& Get(const std::string& filePath, bool load = true)
	{
		auto it = resources.find(filePath);
		if (it != resources.end())
			return *(it->second);

		if (!load || !Load(filePath))
		{
			return Empty;
		}

		return *(resources.find(filePath)->second);
	}

	ResourceMgr(const ResourceMgr&) = delete;
	ResourceMgr(ResourceMgr&&) = delete;

	ResourceMgr& operator=(const ResourceMgr&) = delete;
	ResourceMgr& operator=(ResourceMgr&&) = delete;
};

template<typename T>
T ResourceMgr<T>::Empty;

#define RES_MGR_TEXTURE (ResourceMgr<sf::Texture>::Instance())
#define RES_MGR_FONT (ResourceMgr<sf::Font>::Instance())
#define RES_MGR_SOUND_BUFFER (ResourceMgr<sf::SoundBuffer>::Instance())
#define RES_MGR_ANI_CLIP (ResourceMgr<AnimationClip>::Instance())