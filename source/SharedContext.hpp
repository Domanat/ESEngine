#ifndef SHAREDCONTEXT_HPP
#define SHAREDCONTEXT_HPP

#include "EventManager.hpp"
#include "TextureManager.hpp"
#include "EntityManager.hpp"
#include "Window.hpp"

class Map;

struct SharedContext
{
	SharedContext() :
		window(nullptr),
		eventManager(nullptr),
		textureManager(nullptr),
		entityManager(nullptr),
		gameMap(nullptr)
	{

	}

	Window* window;
	EventManager* eventManager;
	TextureManager* textureManager;
	EntityManager* entityManager;
	Map* gameMap;
};

#endif