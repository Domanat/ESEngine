#ifndef MENUGAMESTATE_HPP
#define MENUGAMESTATE_HPP

#include "BaseState.hpp"
#include "StateManager.hpp"

class MenuState : public BaseState
{
public:
	MenuState(StateManager* stateManager);
	~MenuState();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();

	void MouseClick(EventDetails* details);

private:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Font font;
	sf::Text text;

	sf::Vector2f buttonSize;
	sf::Vector2f buttonPos;
	unsigned int buttonPadding;

	sf::RectangleShape rects[2];
	sf::Text labels[2];
};

#endif