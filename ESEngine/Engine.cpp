#include "Engine.hpp"

Engine::Engine():
	window(sf::VideoMode(windowWidth, windowHeight), label),
	stateManager(&window)
{

	//Set state
}

