#include "Enemy.hpp"
#include "EntityManager.hpp"

Enemy::Enemy(EntityManager* manager) :
	Character(manager), hasDestination(false)
{
	type = EntityType::Enemy;
}

Enemy::~Enemy()
{

}

void Enemy::OnEntityCollision(EntityBase* collider, bool attack)
{
	if (state == EntityState::Dying)
		return;
	if (attack)
		return;

	if (collider->GetType() != EntityType::Player)
		return;

	Character* player = (Character*)collider;
	SetState(EntityState::Attacking);
	player->GetHurt(1);

	if (position.x > player->GetPosition().x)
	{
		player->AddVelocity(-speed.x, 0);
		spriteSheet.SetDirection(Direction::Left);
	}
	else
	{
		player->AddVelocity(speed.x, 0);
		spriteSheet.SetDirection(Direction::Right);
	}
}

void Enemy::Update(float dt)
{
	Character::Update(dt);

	if (hasDestination)
	{
		if (abs(destination.x - position.x) < 16)
		{
			hasDestination = false;
			return;
		}

		if (destination.x - position.x > 0)
		{
			Move(Direction::Right);
		}
		else
		{
			Move(Direction::Left);
		}

		if (collidingOnX)
			hasDestination = false;

		return;
	}

	int random = rand() % 1000 + 1;
	if (random != 1000)
		return;
	int newX = rand() % 65 + 0;

	if (rand() % 2)
		newX = -newX;

	destination.x = position.x + newX;

	if (destination.x < 0)
		destination.x = 0;
	hasDestination = true;
}