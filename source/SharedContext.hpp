#pragma once

#include "EventManager.hpp"
#include "Window.hpp"

struct SharedContext
{
	EventManager* eventManager;
	Window* window;
};