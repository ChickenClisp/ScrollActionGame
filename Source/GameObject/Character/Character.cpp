#include "Character.h"
#include <cmath>

Character::Character()
	: graphic_handle(0)
	, graphic_handles_map({})
	, animtype(AnimType::IDLE)
	, hp(0)
	, animation_frame(0)
	, animation_frame_adjust(0)
	, animation_speed(1) // animation_frame_adjust‚æ‚è‘å‚«‚­‚µ‚Ä,‹N“®Žž‚ÉUpdateAnimation‚Ìif•¶‚ªfalse‚É‚È‚é‚æ‚¤‚É‚·‚é
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
	UpdateAnimation();
}

void Character::UpdateAnimation()
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



void Character::SetAnimation(AnimType new_animtype, int new_animation_speed)
{
	animtype = new_animtype;
	graphic_handle = graphic_handles_map[animtype][animation_frame];
	animation_speed = std::floor(60 / new_animation_speed);
}

void Character::SetFrameZero()
{
	animation_frame = 0;
	animation_frame_adjust = 0;
}



