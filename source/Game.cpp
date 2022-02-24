#include "Game.hpp"

Game::Game() :
	window(sf::Vector2u(800, 600), "Window"),
	stateManager(&sharedContext)
{
	clock.restart();
	srand(time(NULL));

	sharedContext.window = &window;
	sharedContext.eventManager = window.GetEventManager();
	
	stateManager.SwitchTo(StateType::Intro);
	speed = sf::Vector2i(400, 400);
}

Game::~Game() { }

void Game::Update()
{
	window.Update();
	stateManager.Update(elapsed);
}

void Game::Render()
{
	window.BeginDraw();
	//window.Draw(mushroom);
	stateManager.Draw();
	window.EndDraw();
}

void Game::LateUpdate()
{
	stateManager.ProcessRequests();
	RestartClock();
}

Window* Game::GetWindow()
{
	return &window;
}

sf::Time Game::GetElapsed() 
{ 
	return clock.getElapsedTime(); 
}

void Game::RestartClock()
{ 
	elapsed = clock.restart(); 
}