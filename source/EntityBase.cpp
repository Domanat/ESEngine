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