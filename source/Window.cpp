#include "Window.hpp"

Window::Window()
{
	Setup(sf::Vector2u(800, 600), "Window");
}

Window::Window(const sf::Vector2u& size, const std::string& title)
{
	Setup(size, title);
}

Window::~Window()
{
	
}

void Window::Setup(const sf::Vector2u& size, const std::string& title)
{
	windowTitle = title;
	windowSize = size;
	isFullscreen = false;
	isDone = false;
	isFocused = false;
	clearColor = sf::Color::Black;

	eventManager.AddCallback(StateType(0), "Fullscreen_Toggle", &Window::ToggleFullscreen, this);
	eventManager.AddCallback(StateType(0), "Window_Close", &Window::Close, this);

	Create();
}

void Window::Create()
{
	auto style = (isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);

	window.create({ windowSize.x, windowSize.y, 32 }, windowTitle, style);
}

void Window::Update()
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::LostFocus)
		{
			isFocused = false;
			eventManager.SetFocus(false);
		}
		else if (event.type == sf::Event::GainedFocus)
		{
			isFocused = true;
			eventManager.SetFocus(true);
		}

		eventManager.HandleEvent(event);
	}

	eventManager.Update();
}

sf::RenderWindow* Window::GetRenderWindow() { return &window; }
EventManager* Window::GetEventManager() { return &eventManager; }
sf::Vector2u Window::GetWindowSize() { return windowSize; }

sf::FloatRect Window::GetViewSpace()
{
	sf::Vector2f viewCenter = window.getView().getCenter();
	sf::Vector2f viewSize = window.getView().getSize();
	sf::Vector2f viewSizeHalf(viewSize.x / 2, viewSize.y / 2);
	sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);
	return viewSpace;
}

void Window::ToggleFullscreen(EventDetails* details)
{
	isFullscreen = !isFullscreen;
	
	Create();
}

void Window::BeginDraw()
{
	window.clear(clearColor);
}

void Window::EndDraw()
{
	window.display();
}

void Window::Draw(sf::Drawable& drawable)
{
	window.draw(drawable);
}

bool Window::IsDone()
{
	return isDone;
}

bool Window::IsFullscreen()
{
	return isFullscreen;
}

void Window::Close(EventDetails* l_details) 
{ 
	isDone = true; 
}