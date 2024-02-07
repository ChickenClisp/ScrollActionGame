#include "Character.h"
#include "../../Scene/SceneBase.h"
#include "DxLib.h"
#include <cmath>

Character::Character()
	: graphic_handle(0)
	, graphic_handles_map({})
	, animtype(AnimType::IDLE)
	, hp(0)
	, attack_power(0)
	, animation_frame(0)
	, animation_frame_adjust(0)
	, animation_speed(1) // animation_frame_adjustより大きくして,起動時にUpdateAnimationのif文がfalseになるようにする
	, animation_is_loop()
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
	// animation_frameが最後かつループしないアニメーションの場合、何もしない
	if (animation_frame == graphic_handles_map[animtype].size() - 1 && animation_is_loop == false)
	{
		return;
	}
	// 設定したspeedを超えたときに次のanimationに変える
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
	printfDx("%d\n", hp);
	// 死亡判定
	if (hp <= 0)
	{
		OnDead();
	}
}

void Character::OnDead()
{
	printfDx("OnDead\n");
}


void Character::SetAnimation(AnimType new_animtype, int new_animation_speed, bool b_loop)
{
	animtype = new_animtype;
	graphic_handle = graphic_handles_map[animtype][animation_frame];
	animation_speed = std::floor(60 / new_animation_speed);
	animation_is_loop = b_loop;
}

void Character::SetFrameZero()
{
	animation_frame = 0;
	animation_frame_adjust = 0;
}




