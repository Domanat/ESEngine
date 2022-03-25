#include "Map.hpp"
#include "EntityManager.hpp"

Map::Map(SharedContext* sharedContext, BaseState* currState) :
	context(sharedContext), defaultTile(sharedContext), maxMapSize(32, 32),
	tileCount(0), tileSetCount(0), mapGravity(512.f), loadNextMap(false),
	currentState(currState)
{
	context->gameMap = this;
	LoadTiles("tiles.cfg");
}

Map::~Map()
{
	PurgeMap();
	PurgeTileSet();
	context->gameMap = nullptr;
}

Tile* Map::GetTile(unsigned int x, unsigned int y)
{
	auto itr = tileMap.find(ConvertCoords(x, y));
	return (itr != tileMap.end() ? itr->second : nullptr);
}

unsigned int Map::ConvertCoords(unsigned int x, unsigned int y)
{
	return (x * maxMapSize.x) + y;
}

void Map::Update(float dt)
{
	if (loadNextMap)
	{
		PurgeMap();
		loadNextMap = false;

		if (nextMap != "")
		{
			LoadMap("media/maps/" + nextMap);
		}
		else
		{
			//currentState->GetStateManager()->SwitchTo(StateType::GameOver);
		}
		nextMap = "";
	}

	sf::FloatRect viewSpace = context->window->GetViewSpace();
	background.setPosition(viewSpace.left, viewSpace.top);
}

void Map::Draw()
{
	sf::RenderWindow* window = context->window->GetRenderWindow();
	window->draw(background);

	sf::FloatRect viewSpace = context->window->GetViewSpace();

	sf::Vector2i tileBegin(
		floor(viewSpace.left / Sheet::TileSize),
		floor(viewSpace.top / Sheet::TileSize));

	sf::Vector2i tileEnd(
		ceil((viewSpace.left + viewSpace.width) / Sheet::TileSize),
		ceil((viewSpace.top + viewSpace.height) / Sheet::TileSize));

	unsigned int count = 0;

	for (int x = tileBegin.x; x <= tileEnd.x; ++x)
	{
		for (int y = tileBegin.y; y <= tileEnd.y; ++y)
		{
			if (x < 0 || y < 0)
				continue;

			Tile* tile = GetTile(x, y);
			if (!tile)
				continue;

			sf::Sprite& sprite = tile->properties->sprite;
			sprite.setPosition(x * Sheet::TileSize, y * Sheet::TileSize);
			window->draw(sprite);
			++count;
		}
	}
}

void Map::PurgeMap()
{
	tileCount = 0;

	for (auto& itr : tileMap)
	{
		delete itr.second;
	}

	tileMap.clear();
	//context->entityManager->Purge();
	
	if (backgroundTexture == "")
		return;
	context->textureManager->ReleaseResource(backgroundTexture);
	backgroundTexture = "";
}

void Map::PurgeTileSet()
{
	for (auto& itr : tileSet)
	{
		delete itr.second;
	}

	tileSet.clear();
	tileSetCount = 0;
}

void Map::LoadTiles(const std::string& path)
{
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + path);

	if (!file.is_open())
	{
		std::cout << "Error opening file " + path << std::endl;
		return;
	}

	std::string line;

	while (std::getline(file, line))
	{
		if (line[0] == '|')
			continue;

		std::stringstream keystream(line);
		int tileId;
		keystream >> tileId;

		if (tileId < 0)
			continue;

		TileInfo* tile = new TileInfo(context, "TileSheet", tileId);
		keystream >> tile->name >> tile->friction.x >> tile->friction.y >> tile->deadly;

		if (!tileSet.emplace(tileId, tile).second)
		{
			std::cout << "Duplicate file type: " << tile->name << std::endl;
			delete tile;
		}
	}

	file.close();
}

void Map::LoadMap(const std::string& path)
{
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + path);

	if (!file.is_open())
	{
		std::cout << "Error opening file " + path << std::endl;
		return;
	}
	EntityManager* entityMgr = context->entityManager;
	std::string line;

	int playerId = -1;
	while (std::getline(file, line))
	{
		if (line[0] == '|') 
		{ 
			continue;
		}

		std::stringstream keystream(line);
		std::string type;
		keystream >> type;

		if (type == "TILE")
		{
			int tileId = 0;
			keystream >> tileId;

			if (tileId < 0)
			{
				std::cout << "BadTile id: " << tileId << std::endl;
				continue;
			}

			auto itr = tileSet.find(tileId);
			if (itr == tileSet.end())
			{
				std::cout << "Tile id: " << tileId << " wasn't found in tileset" << std::endl;
				continue;
			}

			sf::Vector2i tileCoords;
			keystream >> tileCoords.x >> tileCoords.y;

			if (tileCoords.x > maxMapSize.x || tileCoords.y > maxMapSize.y)
			{
				std::cout << "Tile is out of range " << tileCoords.x << "  " << tileCoords.y << std::endl;
				continue;
			}

			Tile* tile = new Tile();

			tile->properties = itr->second;

			if (!tileMap.emplace(ConvertCoords(tileCoords.x, tileCoords.y), tile).second)
			{
				std::cout << "! Duplicate tile! : " << tileCoords.x << "" << tileCoords.y << std::endl;
				delete tile;
				tile = nullptr;
				continue;
			}

			std::string warp;
			keystream >> warp;
			tile->warp = false;
			if (warp == "WARP")
				tile->warp = true;
		}
		else if (type == "BACKGROUND")
		{
			if (backgroundTexture != "")
				continue;

			keystream >> backgroundTexture;

			if (!context->textureManager->RequireResource(backgroundTexture))
			{
				backgroundTexture = "";
				continue;
			}

			sf::Texture* texture = context->textureManager->GetResources(backgroundTexture);
			background.setTexture(*texture);

			sf::Vector2f viewSize = currentState->GetView().getSize();
			sf::Vector2u textureSize = texture->getSize();
			sf::Vector2f scaleFactors;
			scaleFactors.x = viewSize.x / textureSize.x;
			scaleFactors.y = viewSize.y / textureSize.y;
			background.setScale(scaleFactors);
		}
		else if (type == "SIZE") 
		{
			keystream >> maxMapSize.x >> maxMapSize.y;
		}
		else if (type == "GRAVITY") 
		{
			keystream >> mapGravity;
		}
		else if (type == "DEFAULT_FRICTION") 
		{
			keystream >> defaultTile.friction.x >> defaultTile.friction.y;
		}
		else if (type == "NEXTMAP") 
		{
			keystream >> nextMap;
		}
		else if(type == "PLAYER")
		{
			if (playerId != -1){ continue; }
			// Set up the player position here.
			playerId = entityMgr->Add(EntityType::Player);

			if (playerId < 0){ continue; }

			float playerX = 0; float playerY = 0;
			keystream >> playerX >> playerY;
			entityMgr->Find(playerId)->SetPosition(playerX,playerY);

			playerStart = sf::Vector2f(playerX, playerY);
		} 
		else if(type == "ENEMY")
		{
			std::string enemyName;
			keystream >> enemyName;
			int enemyId = entityMgr->Add(EntityType::Enemy, enemyName);

			if (enemyId < 0){ continue; }

			float enemyX = 0; float enemyY = 0;
			keystream >> enemyX >> enemyY;
			entityMgr->Find(enemyId)->SetPosition(enemyX, enemyY);
		} 
		else 
		{
			// Something else.
			std::cout << "! Unknown type \"" << type << "\"." << std::endl;
		}
	}

	file.close();
}

const sf::Vector2u& Map::GetMapSize() const
{
	return maxMapSize;
}

const sf::Vector2f& Map::GetPlayerStart() const
{
	return playerStart;
}

unsigned int Map::GetTileSize() const
{
	return Sheet::TileSize;
}

TileInfo* Map::GetDefaultTile() 
{ 
	return &defaultTile; 
}

float Map::GetGravity()const 
{ 
	return mapGravity; 
}

void Map::LoadNext() 
{ 
	loadNextMap = true; 
}