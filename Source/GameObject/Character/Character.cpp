#include "Character.h"

Character::Character()
	: graphic_handle(0)
	, graphic_handles_map({})
	, animtype(AnimType::IDLE)
	, hp(0)
	, animation_frame(0)
	, animation_frame_adjust(0)
	, animation_speed(0)
{
}

Character::~Character()
{
}

void Character::Initialize()
{
}

void Character::Update(float delta_seconds)
{
	if (++animation_frame_adjust > animation_speed)
	{
		animation_frame++;
		animation_frame_adjust = 0;
		animation_frame %= graphic_handles_map[animtype].size();
	}
	
}

void Character::Draw(const Vector2D& screen_offset)
{
}

void Character::Finalize()
{
}

void Character::ApplyDamage(int damage)
{
}

void Character::OnDamaged(int damage)
{
}

void Character::ChangeAnimation(AnimType type, int speed)
{
	animtype = type;
	graphic_handle = graphic_handles_map[type][animation_frame];
	animation_speed = speed;
}

void Character::SetFrameZero()
{
	animation_frame = 0;
	animation_frame_adjust = 0;
}

