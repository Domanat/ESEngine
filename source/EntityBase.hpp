#ifndef ENTITY_BASE_HPP
#define ENTITY_BASE_HPP

#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

enum class EntityType { Base, Enemy, Player };

enum class EntityState
{
	Idle, Walking, Jumping, Attacking, Hurt, Dying
};

struct TileInfo;

struct CollisionElement
{
	CollisionElement(float l_area, TileInfo* l_info, const sf::FloatRect& l_bounds)
		:area(l_area), tile(l_info), tileBounds(l_bounds)
	{}

	float area;
	TileInfo* tile;
	sf::FloatRect tileBounds;
};

bool SortCollisions(const CollisionElement& l_1, const CollisionElement& l_2);

class EntityManager;

class EntityBase
{
	friend class EntityManager;
public:
	EntityBase(EntityManager* entityMgr);
	virtual ~EntityBase();

	const sf::Vector2f& GetPosition()const;
	const sf::Vector2f& GetSize()const;
	EntityState GetState()const;
	std::string GetName()const;
	unsigned int GetId()const;
	EntityType GetType()const;

	void SetPosition(float x, float y);
	void SetPosition(const sf::Vector2f& pos);
	void SetSize(float x, float y);
	void SetState(const EntityState& state);

	void Move(float x, float y);
	void AddVelocity(float x, float y);
	void Accelerate(float x, float y);
	void SetAcceleration(float x, float y);
	void ApplyFriction(float x, float y);

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow* wind) = 0;
protected:
	// Methods.
	void UpdateAABB();
	void CheckCollisions();
	void ResolveCollisions();

	// Method for what THIS entity does TO the l_collider entity.
	virtual void OnEntityCollision(EntityBase* collider, bool attack) = 0;

	// Data members.
	std::string name;
	EntityType type;
	unsigned int id; // Entity id in the entity manager.
	sf::Vector2f position; // Current position.
	sf::Vector2f positionOld; // Position before entity moved.
	sf::Vector2f velocity; // Current velocity.
	sf::Vector2f maxVelocity; // Maximum velocity.
	sf::Vector2f speed; // Value of acceleration.
	sf::Vector2f acceleration; // Current acceleration.
	sf::Vector2f friction; // Default friction value.
	TileInfo* referenceTile; // Tile underneath entity.
	sf::Vector2f size; // Size of the collision box.
	sf::FloatRect AABB; // The bounding box for collisions.
	EntityState state; // Current entity state.
	// Flags for remembering axis collisions.
	bool collidingOnX;
	bool collidingOnY;

	std::vector<CollisionElement> collisions;
	EntityManager* entityManager;
};

#endif