#include "EntityBase.hpp"
#include "SharedContext.hpp"
#include "Map.hpp"

EntityBase::EntityBase(EntityManager* entityMgr) :
	entityManager(entityMgr), name("BaseEntity"),
	type(EntityType::Base), referenceTile(nullptr),
	state(EntityState::Idle), id(0),
	collidingOnX(false), collidingOnY(false)
{
	
}

void EntityBase::SetPosition(float x, float y)
{
	position = sf::Vector2f(x, y);
	UpdateAABB();
}

void EntityBase::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	UpdateAABB();
}

void EntityBase::SetSize(float x, float y)
{
	size = sf::Vector2f(x, y);
	UpdateAABB();
}

void EntityBase::SetState(const EntityState& state)
{
	if (this->state == EntityState::Dying)
		return;

	this->state = state;
}

void EntityBase::Move(float x, float y)
{
	positionOld = position;
	position += sf::Vector2f(x, y);

	sf::Vector2u mapSize = entityManager->GetContext()->gameMap->GetMapSize();

	if (position.x < 0)
		position.x = 0;
	else if (position.x > (mapSize.x + 1) * Sheet::TileSize)
		position.x = (mapSize.x + 1) * Sheet::TileSize;

	if (position.y < 0)
		position.y = 0;
	else if (position.y > (mapSize.y + 1) * Sheet::TileSize)
	{
		position.y = (mapSize.y + 1) * Sheet::TileSize;
		SetState(EntityState::Dying);
	}

	UpdateAABB();
}

void EntityBase::AddVelocity(float x, float y)
{
	velocity += sf::Vector2f(x, y);

	if (abs(velocity.x) > maxVelocity.x)
	{
		if (velocity.x < 0)
			velocity.x = -maxVelocity.x;
		else
			velocity.x = maxVelocity.x;
	}

	if (abs(velocity.y) > maxVelocity.y)
	{
		if (velocity.y < 0)
			velocity.y = -maxVelocity.y;
		else
			velocity.y = maxVelocity.y;
	}
}

void EntityBase::Accelerate(float x, float y)
{
	acceleration += sf::Vector2f(x, y);
}

void EntityBase::ApplyFriction(float x, float y)
{
	if (velocity.x != 0)
	{
		if (abs(velocity.x) - abs(x) < 0)
			velocity.x = 0;
		else
		{
			if (velocity.x < 0)
				velocity.x += x;
			else
				velocity.x -= x;
		}

		if (velocity.y != 0) {
			if (abs(velocity.y) - abs(y) < 0) { velocity.y = 0; }
			else 
			{
				if (velocity.y < 0) 
					velocity.y += y;
				else 
					velocity.y -= y;
			}
		}
	}
}

void EntityBase::Update(float dt)
{
	Map* map = entityManager->GetSharedContext()->gameMap;
	float gravity = map->GetGravity();

	Accelerate(0, gravity);
	AddVelocity(acceleration.x * dt, acceleration.y * dt);
	SetAcceleration(0.0f, 0.0f);
	
	sf::Vector2f frictionValue;
	if (referenceTile)
	{
		frictionValue = referenceTile->friction;
		if (referenceTile->deadly)
			SetState(EntityState::Dying);
	}
	else if (map->GetDefaultTile())
	{
		frictionValue = map->GetDefaultTile()->friction;
	}
	else
	{
		frictionValue = this->friction;
	}

	float frictionX = (speed.x * frictionValue.x) * dt;
	float frictionY = (speed.y * frictionValue.y) * dt;

	ApplyFriction(frictionX, frictionY);
	sf::Vector2f deltaPos = this->velocity * dt;

	Move(deltaPos.x, deltaPos.y);

	collidingOnX = false;
	collidingOnY = false;
	CheckCollisions();
	ResolveCollisions();
}

void EntityBase::UpdateAABB()
{
	AABB = sf::FloatRect(position.x - (size.x / 2), position.y - size.y, size.x, size.y);
}

bool SortCollisions(const CollisionElement& first, const CollisionElement& second)
{
	return first.area > second.area;
}

void EntityBase::CheckCollisions()
{
	Map* map = entityManager->GetSharedContext()->gameMap;

	unsigned int tileSize = map->GetTileSize();

	int fromX = floor(AABB.left / tileSize);
	int toX = floor((AABB.left + AABB.width) / tileSize);
	int fromY = floor(AABB.top / tileSize);
	int toY = floor((AABB.top + AABB.height) / tileSize);

	for (int x = fromX; x <= toX; ++x)
	{
		for (int y = fromY; y <= toY; ++y)
		{
			Tile* tile = map->GetTile(x, y);

			if (!tile)
				continue;

			sf::FloatRect tileBounds(x * tileSize, y * tileSize, tileSize, tileSize);
			sf::FloatRect intersection;
			AABB.intersects(tileBounds, intersection);
			float area = intersection.width * intersection.height;

			CollisionElement elem(area, tile->properties, tileBounds);
			collisions.emplace_back(elem);

			if (tile->warp && type == EntityType::Player)
				map->LoadNext();
		}
	}
}

void EntityBase::ResolveCollisions()
{
	if (!collisions.empty())
	{
		std::sort(collisions.begin(), collisions.end(), SortCollisions);
		Map* map = entityManager->GetSharedContext()->gameMap;
		unsigned int tileSize = map->GetTileSize();

		for (auto& itr : collisions)
		{
			if (!AABB.intersects(itr.tileBounds))
				continue;


			float xDiff = (AABB.left + (AABB.width / 2)) - (itr.tileBounds.left + (itr.tileBounds.width / 2));
			float yDiff = (AABB.top + (AABB.height / 2)) - (itr.tileBounds.top + (itr.tileBounds.height / 2));
			float resolve = 0;

			if (abs(xDiff) > abs(yDiff))
			{
				if (xDiff > 0)
				{
					resolve = (itr.tileBounds.left + tileSize) - AABB.left;
				}
				else
					resolve = -((AABB.left + AABB.width) - itr.tileBounds.left);
			
				Move(resolve, 0);
				velocity.x = 0;
				collidingOnX = true;
			}
			else
			{
				if (yDiff > 0)
				{
					resolve = (itr.tileBounds.top + tileSize) - AABB.top;
				}
				else
					resolve = -((AABB.top + AABB.height) - itr.tileBounds.top);

				Move(0, resolve);
				velocity.y = 0;

				if (collidingOnY)
					continue;

				referenceTile = itr.tile;
				collidingOnY = true;
			}
		}

		collisions.clear();
	}

	if (!collidingOnY)
		referenceTile = nullptr;
}