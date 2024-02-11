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
	// ƒXƒNƒŠ[ƒ“À•W‚É•ÏŠ·‚µ‚Ä•`‰æ
	int x, y, screen_offset_x, screen_offset_y;
	GetPosition().ToInt(x, y);
	screen_offset.ToInt(screen_offset_x, screen_offset_y);
	std::string string_player_hp = "HP: " + std::to_string(hp);
	DrawStringF(x - screen_offset_x, y - screen_offset_y - 10, string_player_hp.c_str(), GetColor(255, 255, 255));
}

void Character::Finalize()
{
	__super::Finalize();
}

void Character::ApplyDamage(Character* attack_character, Character* damaged_character)
{
	damaged_character->OnDamaged(attack_character, damaged_character);
}

void Character::OnDamaged(Character* attack_character, class Character* damaged_character)
{
	// hp‚ðUŒ‚—Í•ªŒ¸‚ç‚·
	hp -= attack_character->GetAttackPower();
	// Ž€–S”»’è
	if (hp <= 0)
	{
		OnDead();
	}
}