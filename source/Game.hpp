#ifndef GAME_HPP
#define GAME_HPP

#include "Engine.hpp"
#include <string>
#include <thread>

class Game
{
public:
	Game();

	void Start();
	void Pause();
	void Resume();
	void Stop();

private:
	Engine engine;

	//Keep all textures
	/*std::map<std::string, sf::Texture> backgroundsTextures;
	std::map<std::string, sf::Sprite> backgroundsSprites;

	std::map<std::string, sf::Texture> characters;
	std::map<std::string, sf::Sprite> charactersSprites;*/


};

#endif