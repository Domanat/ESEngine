#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <unordered_map>
#include <functional>
#include "Player.hpp"
#include "Enemy.hpp"

struct SharedContext;

class EntityManager
{
public:
	EntityManager(SharedContext* context, unsigned int maxEntities);
	~EntityManager();

	int Add(const EntityType& type, const std::string& name = "");
	EntityBase* Find(unsigned int id);
	EntityBase* Find(const std::string& name);
	void Remove(unsigned int id);

	void Update(float dt);
	void Draw();

	void Purge();
	SharedContext* GetSharedContext();
private:

	template<class T>
	void RegisterEntry(const EntityType& type)
	{
		entityFactory[type] = [this]()->EntityBase*
		{
			return new T(this);
		};
	}

	void ProcessRemovals();
	void LoadEnemyTypes(const std::string& name);
	void EntityCollisionCheck();

	std::unordered_map<unsigned int, EntityBase*> entities;
	std::unordered_map<std::string, std::string> enemyTypes;
	std::unordered_map<EntityType, std::function<EntityBase* (void)>> entityFactory;
	SharedContext* sharedContext;
	unsigned int idCounter;
	unsigned int maxEntities;

	std::vector<unsigned int> entitiesToRemove;

};

#endif