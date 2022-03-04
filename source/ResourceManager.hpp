#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <unordered_map>
#include <fstream>
#include <iostream>
#include "Utils.hpp"

template <typename Derived, typename T>
class ResourceManager
{
public:
	ResourceManager(const std::string& pathFile)
	{
		LoadPaths(pathFile);
	}

	T* GetResources(const std::string& id)
	{
		auto res = Find(id);
		return (res ? res->first : nullptr);
	}

	std::string GetPath(const std::string& id)
	{
		auto path = paths.find(id);
		return (path != paths.end() ? path->second : "");
	}

	bool RequireResource(const std::string& id)
	{
		auto res = Find(id);
		if (res)
		{
			++res->second;
			return true;
		}

		auto path = paths.find(id);
		if (path == paths.end())
		{
			return false;
		}

		T* resource = Load(path->second);
		if (!resource)
		{
			return false;
		}

		resources.emplace(id, std::make_pair(resource, 1));
		return true;
	}

	bool ReleaseResource(const std::string& id)
	{
		auto res = Find(id);
		if (!res)
			return false;

		--res->second;
		if (!res->second)
			Unload(id);
		return true;
	}

	void PurgeResources()
	{
		while (resources.begin() != resources.end())
		{
			delete resources.begin()->second.first;
			resources.erase(resources.begin());
		}
	}

	T* Load(const std::string& path)
	{
		return static_cast<Derived*>(this)->Load(path);
	}

	virtual ~ResourceManager() { PurgeResources(); }

private:
	std::pair<T*, unsigned int>* Find(const std::string& id)
	{
		auto itr = resources.find(id);
		return (itr != resources.end() ? &itr->second : nullptr);
	}

	bool Unload(const std::string& id)
	{
		auto itr = resources.find(id);
		if (itr == resources.end())
			return false;

		delete itr->second.first;
		resources.erase(itr);
		return true;
	}

	void LoadPaths(const std::string& pathString)
	{
		std::ifstream fileStream;

		fileStream.open(Utils::GetWorkingDirectory() + pathString);

		if (fileStream.is_open())
		{
			std::string line;
			while (std::getline(fileStream, line))
			{
				std::stringstream keystream(line);
				std::string pathName;
				std::string path;

				keystream >> pathName;
				keystream >> path;
				paths.emplace(pathName, path);
			}

			fileStream.close();
			return;
		}

		std::cerr << "Failed to load the path file " << pathString << std::endl;
	}

	std::unordered_map<std::string, std::pair<T*, unsigned int>> resources;
	std::unordered_map<std::string, std::string> paths;
};

#endif