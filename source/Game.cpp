#include "Game.hpp"

Game::Game() :
	window(sf::Vector2u(800, 600), "Window"),
	stateManager(&sharedContext),
	entityManager(&sharedContext, 10)
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