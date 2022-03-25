#include "Character.hpp"
#include "EntityManager.hpp"
#include "TextureManager.hpp"

Character::Character(EntityManager* manager) :
	EntityBase(manager),
	spriteSheet(entityManager->GetSharedContext()->textureManager),
	jumpVelocity(250), hitpoints(5)
{
	name = "Character";
}

void Character::Move(const Direction& dir)
{
	if (GetState() == EntityState::Dying)
		return;

	spriteSheet.SetDirection(dir);

	if (dir == Direction::Left)
		Accelerate(-speed.x, 0);
	else
		Accelerate(speed.x, 0);

	if (GetState() == EntityState::Idle)
		SetState(EntityState::Walking);
}

void Character::Jump()
{
	if (GetState() == EntityState::Dying || GetState() == EntityState::Jumping ||
		GetState() == EntityState::Hurt)
		return;

	SetState(EntityState::Jumping);
	AddVelocity(0, -jumpVelocity);

}

void Character::Attack()
{
	if (GetState() == EntityState::Dying || GetState() == EntityState::Jumping ||
		GetState() == EntityState::Hurt || GetState() == EntityState::Attacking)
		return;

	SetState(EntityState::Attacking);
}

void Character::GetHurt(const int& damage)
{
	if (GetState() == EntityState::Dying ||
		GetState() == EntityState::Hurt)
		return;

	hitpoints = (hitpoints - damage > 0 ? hitpoints - damage : 0);

	if (hitpoints)
		SetState(EntityState::Hurt);
	else
		SetState(EntityState::Dying);
}

void Character::Load(const std::string& path) 
{
	std::ifstream file;

	file.open(Utils::GetResourceDirectory() + std::string("media/Characters/") + path);
	
	if (!file.is_open()) 
	{
		std::cout << "! Failed loading the character file: " << path << std::endl;
		return; 
	}

	std::string line;

	while (std::getline(file, line)) 
	{
		if (line[0] == '|') { continue; }

		std::stringstream keystream(line);
		std::string type;
		keystream >> type;

		if (type == "Name") 
		{
			keystream >> name;
		}
		else if (type == "Spritesheet") 
		{
			std::string path;
			keystream >> path;
			spriteSheet.LoadSheet("media/Spritesheets/" + path);
		}
		else if (type == "Hitpoints") 
		{
			keystream >> hitpoints;
		}
		else if (type == "BoundingBox") 
		{
			sf::Vector2f boundingSize;
			keystream >> boundingSize.x >> boundingSize.y;
			SetSize(boundingSize.x, boundingSize.y);
		}
		else if (type == "DamageBox") 
		{
			keystream >> attackAABBoffset.x >> attackAABBoffset.y
				>> attackAABB.width >> attackAABB.height;
		}
		else if (type == "Speed") 
		{
			keystream >> speed.x >> speed.y;
		}
		else if (type == "JumpVelocity") 
		{
			keystream >> jumpVelocity;
		}
		else if (type == "MaxVelocity") 
		{
			keystream >> maxVelocity.x >> maxVelocity.y;
		}
		else 
		{
			std::cout << "! Unknown type in character file: " << type << std::endl;
		}
	}
	file.close();
}

void Character::UpdateAttackAABB()
{
	attackAABB.left = (spriteSheet.GetDirection() == Direction::Left ?
		(AABB.left - attackAABB.width) - attackAABBoffset.x :
		(AABB.left + AABB.width) + attackAABBoffset.x);

	attackAABB.top = AABB.top + attackAABBoffset.x;
}

void Character::Animate()
{
	EntityState state = GetState();

	if (state == EntityState::Walking && spriteSheet.GetCurrentAnimation()->GetName() != "Walk")
		spriteSheet.SetAnimation("Walk", true, true);
	else if (state == EntityState::Jumping && spriteSheet.GetCurrentAnimation()->GetName() != "Jump")
		spriteSheet.SetAnimation("Jump", true, false);
	else if (state == EntityState::Attacking && spriteSheet.GetCurrentAnimation()->GetName() != "Attack")
		spriteSheet.SetAnimation("Attack", true, false);
	else if (state == EntityState::Hurt && spriteSheet.GetCurrentAnimation()->GetName() != "Hurt")
		spriteSheet.SetAnimation("Hurt", true, false);
	else if (state == EntityState::Dying && spriteSheet.GetCurrentAnimation()->GetName() != "Death")
		spriteSheet.SetAnimation("Death", true, false);
	else if (state == EntityState::Idle && spriteSheet.GetCurrentAnimation()->GetName() != "Idle")
		spriteSheet.SetAnimation("Idle", true, true);
}

void Character::Update(float dt)
{
	EntityBase::Update(dt);

	if (attackAABB.width != 0 && attackAABB.height != 0)
		UpdateAttackAABB();

	if (GetState() != EntityState::Dying && GetState() !=
		EntityState::Attacking && GetState() != EntityState::Hurt)
	{
		if (abs(velocity.y) >= 0.001f)
			SetState(EntityState::Jumping);
		else if (abs(velocity.x) >= 0.1f)
			SetState(EntityState::Walking);
		else
			SetState(EntityState::Idle);
	}
	else if (GetState() == EntityState::Attacking || GetState() == EntityState::Hurt)
	{
		if (!spriteSheet.GetCurrentAnimation()->IsPlaying())
			SetState(EntityState::Idle);
	}
	else if (GetState() == EntityState::Dying)
	{
		if (!spriteSheet.GetCurrentAnimation()->IsPlaying())
			entityManager->Remove(id);
	}

	Animate();
	spriteSheet.Update(dt);
	spriteSheet.SetSpritePosition(position);
}

void Character::Draw(sf::RenderWindow* window)
{
	spriteSheet.Draw(window);
}