#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "EntityBase.hpp"
#include "SpriteSheet.hpp"

class Character : public EntityBase
{
	friend class EntityManager;
public:
	Character(EntityManager* manager);

	void Move(const Direction& dir);

	void Jump();
	void Attack();
	void GetHurt(const int& damage);

	void Load(const std::string& path);

	virtual void OnEntityCollision(EntityBase* collider, bool attack) = 0;
	virtual void Update(float dt);

	void Draw(sf::RenderWindow* wnd);

protected:
	void UpdateAttackAABB();
	void Animate();

	SpriteSheet spriteSheet;
	float jumpVelocity;
	int hitpoints;
	sf::FloatRect attackAABB;
	sf::Vector2f attackAABBoffset;
};

#endif 