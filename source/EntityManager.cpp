#include "EntityManager.hpp"
#include "SharedContext.hpp"

EntityManager::EntityManager(SharedContext* context, unsigned int maxEntities) :
	sharedContext(context), maxEntities(maxEntities), idCounter(0)
{
	LoadEnemyTypes("EnemyList.list");
	RegisterEntry<Player>(EntityType::Player);
	RegisterEntry<Enemy>(EntityType::Enemy);
}

EntityManager::~EntityManager()
{
	Purge();
}


int EntityManager::Add(const EntityType& type, const std::string& name)
{
	auto itr = entityFactory.find(type);

	if (itr == entityFactory.end())
		return -1;

	EntityBase* entity = itr->second();
	entity->id = idCounter;

	if (name != "")
		entity->name = name;
	entities.emplace(idCounter, entity);
	
	if (type == EntityType::Enemy)
	{
		auto itr = enemyTypes.find(name);
		if (itr != enemyTypes.end())
		{
			Enemy* enemy = (Enemy*) entity;
			enemy->Load(itr->second);
		}
	}

	++idCounter;
	return idCounter - 1;
}

EntityBase* EntityManager::Find(unsigned int id)
{
	auto itr = entities.find(id);
	if (itr == entities.end())
		return nullptr;

	return itr->second;
}

EntityBase* EntityManager::Find(const std::string& name)
{
	for (auto& itr : entities)
	{
		if (itr.second->GetName() == name)
			return itr.second;
	}

	return nullptr;
}

void EntityManager::Remove(unsigned int id)
{
	entitiesToRemove.emplace_back(id);
}

void EntityManager::Update(float dt)
{
	for (auto& itr : entities)
	{
		itr.second->Update(dt);
	}

	EntityCollisionCheck();
	ProcessRemovals();
}

void EntityManager::Draw()
{
	sf::RenderWindow* window = sharedContext->window->GetRenderWindow();
	sf::FloatRect viewSpace = sharedContext->window->GetViewSpace();

	for (auto& itr : entities)
	{
		if (!viewSpace.intersects(itr.second->AABB))
			continue;

		itr.second->Draw(window);
	}
}

void EntityManager::Purge()
{
	for (auto& itr : entities)
	{
		delete itr.second;
	}

	entities.clear();
	idCounter = 0;
}

void EntityManager::ProcessRemovals()
{
	while (entitiesToRemove.begin() != entitiesToRemove.end())
	{
		unsigned int id = entitiesToRemove.back();
		auto itr = entities.find(id);

		if (itr != entities.end())
		{
			std::cout << "Discarding entry " << itr->second->GetId() << std::endl;
			delete itr->second;
			entities.erase(itr);
		}

		entitiesToRemove.pop_back();
	}
}

void EntityManager::EntityCollisionCheck()
{
	if (entities.empty())
		return;

	for (auto itr1 = entities.begin(); std::next(itr1) != entities.end(); ++itr1)
	{
		for (auto itr2 = std::next(itr1); itr2 != entities.end(); ++itr2)
		{
			if (itr1->first == itr2->first)
				continue;

			//Regular AABB bounding box collision
			if (itr1->second->AABB.intersects(itr2->second->AABB))
			{
				itr1->second->OnEntityCollision(itr2->second, false);
				itr2->second->OnEntityCollision(itr1->second, false);
			}

			EntityType t1 = itr1->second->GetType();
			EntityType t2 = itr2->second->GetType();

			if (t1 == EntityType::Player || t1 == EntityType::Enemy)
			{
				Character* c1 = (Character*)itr1->second;

				if (c1->attackAABB.intersects(itr2->second->AABB))
					c1->OnEntityCollision(itr2->second, true);
			}


			if (t2 == EntityType::Player || t2 == EntityType::Enemy)
			{
				Character* c2 = (Character*)itr2->second;

				if (c2->attackAABB.intersects(itr2->second->AABB))
					c2->OnEntityCollision(itr1->second, true);
			}
		}
	}
}

void EntityManager::LoadEnemyTypes(const std::string& name)
{
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + std::string("media/characters/") + name);

	if (!file.is_open())
	{
		std::cout << "failed to load file " << name << std::endl;
		return;
	}

	std::string line;

	while (std::getline(file, line))
	{
		if (line[0] == '|')
			continue;

		std::stringstream keystream(line);
		std::string name;
		std::string charFile;
		keystream >> name >> charFile;
		enemyTypes.emplace(name, charFile);
	}

	file.close();
		
}


SharedContext* EntityManager::GetSharedContext() 
{ 
	return sharedContext; 
}