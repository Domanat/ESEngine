#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <map>
#include <sstream>
#include <fstream>
#include "Utils.hpp"
#include "SharedContext.hpp"
#include "BaseState.hpp"

enum Sheet {TileSize = 32, SheetWidth = 256, SheetHeight = 256};

struct TileInfo
{
	TileInfo(SharedContext* context, const std::string& texture = "", int id = 0) :
		context(context), id(0), deadly(false)
	{
		TextureManager* textureManager = context->textureManager;

		if (texture == "")
		{
			this->id = id;
			return;
		}

		if (!textureManager->GetResources(texture))
			return;

		this->texture = texture;
		this->id = id;
		sprite.setTexture(*textureManager->GetResources(texture));
		sf::IntRect tileBoundaries(this->id % (Sheet::SheetWidth / Sheet::TileSize) * Sheet::TileSize,
			this->id / (Sheet::SheetHeight / Sheet::TileSize) * Sheet::TileSize,
			Sheet::TileSize, Sheet::TileSize);

		sprite.setTextureRect(tileBoundaries);
	}

	~TileInfo()
	{
		if (this->texture == "")
			return;

		context->textureManager->ReleaseResource(texture);
	}

	sf::Sprite sprite;
	int id;
	std::string name;
	sf::Vector2f friction;
	bool deadly;

	SharedContext* context;
	std::string texture;
};

struct Tile
{
	TileInfo* properties;
	bool warp;
};

class Map
{
public:
	Map(SharedContext* context, BaseState* currentState);
	~Map();
	Tile* GetTile(unsigned int x, unsigned int y);
	TileInfo* GetDefaultTile();
	float GetGravity() const;
	unsigned int GetTileSize() const;
	const sf::Vector2u& GetMapSize() const;
	const sf::Vector2f& GetPlayerStart() const;
	void LoadMap(const std::string& path);
	void LoadNext();
	void Update(float dt);
	void Draw();

private:
	unsigned int ConvertCoords(unsigned int x, unsigned int y);
	void LoadTiles(const std::string& path);
	void PurgeMap();
	void PurgeTileSet();

	std::unordered_map<int, TileInfo*> tileSet;
	std::unordered_map<int, Tile*> tileMap;
	sf::Sprite background;
	TileInfo defaultTile;
	sf::Vector2u maxMapSize;
	sf::Vector2f playerStart;
	unsigned int tileCount;
	unsigned int tileSetCount;
	float mapGravity;
	std::string nextMap;
	bool loadNextMap;
	std::string backgroundTexture;
	BaseState* currentState;
	SharedContext* context;
};

#endif