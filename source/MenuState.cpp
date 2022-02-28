#include "MenuState.hpp"

MenuState::MenuState(StateManager* stateManager) :
	BaseState(stateManager)
{

}

MenuState::~MenuState()
{

}

void MenuState::OnCreate()
{
	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setString(sf::String("MAIN MENU"));
	text.setCharacterSize(18);

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2,
				   textRect.top + textRect.height / 2);

	text.setPosition(400, 100);

	buttonSize = sf::Vector2f(300, 32);
	buttonPos = sf::Vector2f(400, 200);
	buttonPadding = 4; // 4px

	std::string str[3];
	str[0] = "PLAY";
	str[1] = "CREDITS";
	str[2] = "EXIT";

	for (int i = 0; i < 3; i++)
	{
		sf::Vector2f buttonPosition(buttonPos.x, buttonPos.y + (i * (buttonSize.y + buttonPadding)));
		
		rects[i].setSize(buttonSize);
		rects[i].setFillColor(sf::Color::Red);

		rects[i].setOrigin(buttonSize.x / 2.0f, buttonSize.y / 2.0f);
		rects[i].setPosition(buttonPosition);

		labels[i].setFont(font);
		labels[i].setString(sf::String(str[i]));
		labels[i].setCharacterSize(12);

		sf::FloatRect rect = labels[i].getLocalBounds();
		labels[i].setOrigin(rect.left + rect.width / 2, rect.top + rect.height / 2);

		labels[i].setPosition(buttonPosition);
	}

	EventManager* eventManager = stateManager->GetSharedContext()->eventManager;

	eventManager->AddCallback(StateType::MainMenu, "Mouse_Left", &MenuState::MouseClick, this);

}

void MenuState::OnDestroy()
{
	EventManager* eventManager = stateManager->GetSharedContext()->eventManager;

	eventManager->RemoveCallback(StateType::MainMenu, "Mouse_Left");
}

void MenuState::Activate()
{

	if (stateManager->HasState(StateType::Game) && labels[0].getString() == "PLAY")
	{
		labels[0].setString(sf::String("RESUME"));
		sf::FloatRect rect = labels[0].getLocalBounds();
		labels[0].setOrigin(rect.left + rect.width / 2, rect.top + rect.height / 2);
	}
}

void MenuState::Deactivate()
{

}

void MenuState::MouseClick(EventDetails* details)
{
	sf::Vector2i mousePosition = details->m_mouse;

	float halfX = buttonSize.x / 2;
	float halfY = buttonSize.y / 2;

	for (int i = 0; i < 3; i++)
	{
		if (mousePosition.x >= rects[i].getPosition().x - halfX &&
			mousePosition.x <= rects[i].getPosition().x + halfX &&
			mousePosition.y >= rects[i].getPosition().y - halfY &&
			mousePosition.y <= rects[i].getPosition().y + halfY)
		{
			if (i == 0)
			{
				stateManager->SwitchTo(StateType::Game);
			}
			else if (i == 1)
			{

			}
			else if (i == 2)
			{
				stateManager->GetSharedContext()->window->Close();
			}
		}
	}
}

void MenuState::Update(const sf::Time& time)
{

}

void MenuState::Draw()
{
	sf::RenderWindow* window = stateManager->GetSharedContext()->window->GetRenderWindow();

	window->draw(text);

	for (int i = 0; i < 3; i++)
	{
		window->draw(rects[i]);
		window->draw(labels[i]);
	}
}