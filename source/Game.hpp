#ifndef GAME_HPP
#define GAME_HPP

//#include "Window.h"
//#include "EventManager.h"
#include "StateManager.hpp"
#include <iostream>

class Game 
{
public:
	Game();
	~Game();

	void Update();
	void Render();
	void LateUpdate();

	sf::Time GetElapsed();

	sf::RenderWindow* GetWindow();
private:

	void RestartClock();

	sf::RenderWindow window;
	StateManager stateManager;
	sf::Clock clock;
	sf::Time elapsed;
};
#endif