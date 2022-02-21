#ifndef GAME_HPP
#define GAME_HPP

//#include "Window.h"
//#include "EventManager.h"
#include "StateManager.hpp"
#include "Window.hpp"
#include <iostream>

struct SharedContext
{
	EventManager* eventManager;
	Window* window;
};

class Game 
{
public:
	Game();
	~Game();

	void Update();
	void Render();
	void LateUpdate();

	sf::Time GetElapsed();

	Window* GetWindow();
	void RestartClock();

private:

	StateManager stateManager;
	Window window;
	SharedContext sharedContext;
	sf::Clock clock;
	sf::Time elapsed;
	sf::Vector2i speed;
};
#endif