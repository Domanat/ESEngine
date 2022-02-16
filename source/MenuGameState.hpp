#ifndef MENUGAMESTATE_HPP
#define MENUGAMESTATE_HPP

#include "GameState.hpp"

class MenuGameState : public GameState
{
public:
	MenuGameState(sf::RenderWindow* window);
	void Init();
	void HandleInput(sf::Event event);
	void Draw();
	void CleanUp();

	void Pause();
	void Resume();

private:

	//Labels textures etc.
	
};

#endif