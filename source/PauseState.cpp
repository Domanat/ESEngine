#include "PauseState.hpp"

PauseState::PauseState(StateManager* stateManager) :
	BaseState(stateManager)
{

}

PauseState::~PauseState()
{

}

void PauseState::OnCreate()
{
	SetTransparent(true);
	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setString(sf::String("PAUSED"));
	text.setCharacterSize(14);
	text.setStyle(sf::Text::Bold);

	sf::Vector2u windowSize = stateManager->GetSharedContext()->window->GetRenderWindow()->getSize();

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
	text.setPosition(windowSize.x / 2, windowSize.y / 2);

	rect.setSize(sf::Vector2f(windowSize));
	rect.setPosition(0, 0);
	rect.setFillColor(sf::Color(0, 0, 0, 150));

	EventManager* eventManager = stateManager->GetSharedContext()->eventManager;

	eventManager->AddCallback(StateType::Paused, "Key_P", &PauseState::Unpause, this);
}

void PauseState::OnDestroy()
{
	EventManager* evManager = stateManager->GetSharedContext()->eventManager;
	evManager->RemoveCallback(StateType::Paused, "Key_P");
}

void PauseState::Draw()
{
	sf::RenderWindow* window = stateManager->GetSharedContext()->window->GetRenderWindow();

	window->draw(rect);
	window->draw(text);
}

void PauseState::Unpause(EventDetails* details)
{
	stateManager->SwitchTo(StateType::Game);
}

void PauseState::Activate() {}
void PauseState::Deactivate() {}
void PauseState::Update(const sf::Time& time) {}