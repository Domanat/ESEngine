#pragma once

#include "EventManager.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"

class Map;

struct SharedContext
{
	EventManager* eventManager;
	TextureManager* textureManager;
	Window* window;
	Map* gameMap;
};