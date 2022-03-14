#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>
#include "EventManager.hpp"
#include <string>

class Window
{
public:
	Window();
	Window(const sf::Vector2u& size, const std::string& title);
	~Window();

	void BeginDraw();
	void EndDraw();

	void Update();

	bool IsDone();
	bool IsFullscreen();

	sf::Vector2u GetWindowSize();
	sf::RenderWindow* GetRenderWindow();
	EventManager* GetEventManager();
	sf::FloatRect GetViewSpace();

	void ToggleFullscreen(EventDetails* details);
	void Close(EventDetails* details = nullptr);

	void Draw(sf::Drawable& drawable);
private:
	void Setup(const sf::Vector2u& size, const std::string& title);
	void Create();

	sf::RenderWindow window;
	EventManager eventManager;
	sf::Vector2u windowSize;
	std::string windowTitle;
	bool isFullscreen;
	bool isDone;
	bool isFocused;

	sf::Color clearColor;
};

#endif