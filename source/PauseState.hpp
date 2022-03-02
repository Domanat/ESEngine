#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "BaseState.hpp"
#include "StateManager.hpp"

class PauseState : public BaseState
{
public:
	PauseState(StateManager* stateManager);
	~PauseState();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();

	void Unpause(EventDetails* details);

private:
	sf::Font font;
	sf::Text text;
	sf::RectangleShape rect;
};

#endif