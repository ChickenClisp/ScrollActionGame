#include "Character.h"
#include "DxLib.h"
#include <cmath>

Character::Character()
	: graphic_handle(0)
	, graphic_handles_map({})
	, animtype(AnimType::IDLE)
	, hp(0)
	, animation_frame(0)
	, animation_frame_adjust(0)
	, animation_speed(1) // animation_frame_adjust���傫������,�N������UpdateAnimation��if����false�ɂȂ�悤�ɂ���
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
	__super::Draw(screen_offset);
	// ���݂̃A�j���[�V�������擾
	graphic_handle = graphic_handles_map[animtype][animation_frame];
	// �X�N���[�����W�ɕϊ����ĕ`��
	int x, y, screen_offset_x, screen_offset_y;
	GetPosition().ToInt(x, y);
	screen_offset.ToInt(screen_offset_x, screen_offset_y);
	if (current_direction == Direction::FRONT)
	{
		DrawGraph(x - screen_offset_x, y - screen_offset_y, graphic_handle, true);
	}
	else
	{
		DrawTurnGraph(x - screen_offset_x, y - screen_offset_y, graphic_handle, true);
	}
}

void Character::Finalize()
{
	__super::Finalize();
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




