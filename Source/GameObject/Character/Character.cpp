#include "Character.h"
#include "../../Scene/SceneBase.h"
#include "DxLib.h"

Character::Character()
	: hp(0)
	, attack_power(0)
	, current_direction()
	, current_isground()
	, verocity({})
{
}

Character::~Character()
{
	Finalize();
}

void Character::Initialize()
{
	__super::Initialize();
	current_direction = Direction::FRONT;
	current_isground = IsGround::OnGround;
}

void Character::Update(float delta_seconds)
{
	__super::Update(delta_seconds);
	UpdateAnimation();
}


void Character::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);
}

void Character::Finalize()
{
	__super::Finalize();
}

void Character::ApplyDamage(int damage, Character* damaged_character)
{
	damaged_character->OnDamaged(damage, damaged_character);
}

void Character::OnDamaged(int damage, class Character* damaged_character)
{
	hp -= damage;
	if (damaged_character->body_collision_params.collision_object_type == CollisionObjectType::PLAYER)
	{
		printfDx("Player's HP:%d\n", hp);
	}
	if (damaged_character->body_collision_params.collision_object_type == CollisionObjectType::ENEMY)
	{
		printfDx("Enemy's HP:%d\n", hp);
	}
	// Ž€–S”»’è
	if (hp <= 0)
	{
		OnDead();
	}
}

void Character::OnDead()
{
	printfDx("OnDead\n");
}