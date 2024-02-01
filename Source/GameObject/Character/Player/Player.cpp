#include "Player.h"

#include "DxLib.h"
#include "../Source/Utility/GraphicResourceManager.h"
#include "../../../SystemTypes.h"
#include "../../Ground/Ground.h"
#include "../Source/Scene/SceneBase.h"
#include "../Source/Scene/IngameScene/IngameScene.h"

Player::Player()
	: current_player_state()
	, equipped_sword(nullptr)
	, is_invincible()
	, invincible_timer()
	, key()
{
}

Player::~Player()
{
	Finalize();
}

void Player::Initialize()
{
	__super::Initialize();
	// 画像の読み込み
	GraphicResourceManager& graphic_resource_manager = GraphicResourceManager::GetInstance();
	std::vector<int> out_sprite_handles;
	/*
	// IDLE
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/collon_wait_a.bmp"), 4, 4, 1, 128, 128, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::IDLE, out_sprite_handles);
	// RUN
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/collon_run8.bmp"), 8, 4, 2, 128, 128, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::RUN, out_sprite_handles);
	// JUMP
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/collon_jump.bmp"), 6, 4, 2, 128, 128, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::JUMP, out_sprite_handles);
	// ATTACK
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/collon_attack.bmp"), 3, 3, 1, 128, 128, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::ATTACK, out_sprite_handles);
	//DAMAGE
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/collon_damage.bmp"), 1, 1, 1, 128, 128, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::DAMAGED, out_sprite_handles);
	*/

	// IDLE
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/Player/player_idle.png"), 4, 4, 1, 50, 50, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::IDLE, out_sprite_handles);
	// RUN
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/Player/player_run.png"), 6, 6, 1, 50, 50, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::RUN, out_sprite_handles);
	// JUMP
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/Player/player_jump.png"), 4, 4, 1, 50, 50, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::JUMP, out_sprite_handles);
	// ATTACK
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/Player/player_attack.png"), 6, 6, 1, 50, 50, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::ATTACK, out_sprite_handles);
	//DAMAGE
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/Player/player_hurt.png"), 3, 3, 1, 50, 50, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::DAMAGED, out_sprite_handles);

	
	// Playerメンバ変数の初期化
	current_player_state = PlayerState::RUN; // SetAnimation()するために異なるステートを宣言
	ChangePlayerState(PlayerState::IDLE);
	current_direction = Direction::FRONT;
	current_isground = IsGround::OnGround;

	center_dir = Vector2D(25.0f, 25.0f);
	body_collision_params = { Vector2D{GetPosition() + center_dir }, Vector2D{25, 40}, CollisionObjectType::PLAYER , 0, CollisonType::BLOCK };
	hp = 10;
	attack_power = 1;
}

void Player::Update(float delta_seconds)
{
	__super::Update(delta_seconds);
	const float MAX_SPEED = 300.0f;
	const float JUMP_POWER = 800.0f;
	const float GRAVITY = 50.0f;
	UpdateInput();
	UpdateInvincibleTimer();

	prev_position = GetPosition();

	// もしAorDが押されていなかったら、速度を下げる
	if (!key[KEY_INPUT_A] && !key[KEY_INPUT_D])
	{
		if (verocity.x < 0)
		{
			verocity.x += 30.0f;
		}
		else if(verocity.x > 0)
		{
			verocity.x -= 30.0f;
		}

		if (abs(verocity.x) < 30.0f)
		{
			verocity.x = 0.0f;
		}
	}
	// もしAが押されていたら、左向きに速度を上げる
	else if (key[KEY_INPUT_A])
	{
		verocity.x -= 10.0f;
		if (abs(verocity.x) > MAX_SPEED)
		{
			verocity.x = -MAX_SPEED;
		}
		current_direction = Direction::BACK;
	}
	// もしDが押されていたら、右向きに速度を上げる
	else if (key[KEY_INPUT_D])
	{
		verocity.x += 10.0f;
		if (abs(verocity.x) > MAX_SPEED)
		{
			verocity.x = MAX_SPEED;
		}
		current_direction = Direction::FRONT;
	}
	// もしSPACEが押されたら、ジャンプ
	if (key[KEY_INPUT_SPACE]==1 && current_isground == IsGround::OnGround)
	{
		verocity.y -= JUMP_POWER;
		current_isground = IsGround::InAir;
		ChangePlayerState(PlayerState::JUMP);
	}
	// もしEが押されてたら、攻撃
	if (key[KEY_INPUT_E] == 1)
	{
		ChangePlayerState(PlayerState::ATTACK);
	}
	
	// PlayerStateの遷移条件
	switch (current_player_state){
	case PlayerState::IDLE:
		if (abs(verocity.x) > 0)
		{
			ChangePlayerState(PlayerState::RUN);
		}
		break;
	case PlayerState::RUN:
		if (verocity.x == 0.0f)
		{
			ChangePlayerState(PlayerState::IDLE);	
		}
		break;
	case PlayerState::JUMP:
		// JUMPのアニメーションが終われば
		if (animation_frame == graphic_handles_map[AnimType::JUMP].size() - 1)
		{
			ChangePlayerState(PlayerState::IDLE);
		}
		break;
	case PlayerState::ATTACK:
		// ATTACK中は移動量を下げる
		verocity.x *= 0.80f;
		if (abs(verocity.x) < 50.0f) verocity.x = 0.0f;
		// ATTACKのアニメーションが終われば
		if (animation_frame == graphic_handles_map[AnimType::ATTACK].size()-1)
		{
			ChangePlayerState(PlayerState::IDLE);
		}
		break;
	case PlayerState::DAMAGE:
		// DAMAGEのアニメーションが終われば
		if (animation_frame == graphic_handles_map[AnimType::DAMAGED].size() - 1)
		{
			ChangePlayerState(PlayerState::IDLE);
		}
		break;
	}

	verocity.y += GRAVITY;

	// 移動ベクトルを求める	
	delta_position = verocity * delta_seconds;
	// 新しいx座標がステージ外であった場合、移動ベクトルを0にする
	if (prev_position.x + delta_position.x < - (128 - (64 + body_collision_params.box_extent.x / 2)) ||
		(prev_position.x + delta_position.x) > (owner_scene->stage_size.x - (128- (64 - body_collision_params.box_extent.x / 2))))
	{
		delta_position.x = 0.0f;
	}
	// 座標の更新
	SetPosition(prev_position + delta_position);
	UpdateCollisionParamsCenterPosition(this);
}


void Player::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);
	// 現在のアニメーションを取得
	graphic_handle = graphic_handles_map[animtype][animation_frame];
	// スクリーン座標に変換して描画
	int x, y, screen_offset_x, screen_offset_y;
	GetPosition().ToInt(x, y);
	screen_offset.ToInt(screen_offset_x, screen_offset_y);
	if (current_direction == Direction::FRONT)
	{
		if (is_invincible == true)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawGraph(x - screen_offset_x, y - screen_offset_y, graphic_handle, true);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		}
		else
		{
			DrawGraph(x - screen_offset_x, y - screen_offset_y, graphic_handle, true);
		}
	}
	else
	{
		if (is_invincible == true)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawTurnGraph(x - screen_offset_x, y - screen_offset_y, graphic_handle, true);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		}
		else
		{
			DrawTurnGraph(x - screen_offset_x, y - screen_offset_y, graphic_handle, true);
		}
		
	}

	// デバッグ用　コリジョンの表示
	DrawBox(x - screen_offset_x, y - screen_offset_y, x - screen_offset_x + 50, y - screen_offset_y + 50, GetColor(0, 0, 255), false);
	DrawBox(body_collision_params.center_position.x - (body_collision_params.box_extent.x / 2 - 1) - screen_offset_x,
		body_collision_params.center_position.y - (body_collision_params.box_extent.y / 2 - 1) - screen_offset_y,
		body_collision_params.center_position.x + (body_collision_params.box_extent.x / 2 - 1) - screen_offset_x,
		body_collision_params.center_position.y + (body_collision_params.box_extent.y / 2 - 1) - screen_offset_y,
		GetColor(0, 255, 0),
		false);	
}

void Player::Finalize()
{
	__super::Finalize();

	// 画像の破棄
	DeleteGraph(graphic_handle);
	graphic_handle = 0;
}

void Player::ChangePlayerState(PlayerState new_state)
{
	if (new_state != current_player_state)
	{
		SetFrameZero();
		OnLeavePlayerState(current_player_state);
		current_player_state = new_state;
		OnEnterPlayerState(current_player_state);
	}
}

void Player::OnEnterPlayerState(PlayerState state)
{
	const int ANIMATION_SPPED_IDLE = 6;
	const int ANIMATION_SPPED_RUN = 12;
	const int ANIMATION_SPPED_JUMP = 10;
	const int ANIMATION_SPPED_ATTACK = 16;
	const int ANIMATION_SPPED_DAMAGED = 4;
	switch (state) {
	case PlayerState::IDLE:
		SetAnimation(AnimType::IDLE, ANIMATION_SPPED_IDLE);
		break;

	case PlayerState::RUN:
		SetAnimation(AnimType::RUN, ANIMATION_SPPED_RUN);
		break;

	case PlayerState::JUMP:
		SetAnimation(AnimType::JUMP, ANIMATION_SPPED_JUMP);
		break;

	case PlayerState::ATTACK:
		SetAnimation(AnimType::ATTACK, ANIMATION_SPPED_ATTACK);
		// swordを有効化して、swordの位置をプレイヤーの前に変更する
		equipped_sword->SetActive(true);
		if (current_direction == Direction::FRONT)
		{
			equipped_sword->SetPosition(this->GetPosition() + Vector2D(35.0f, 5.0f));
			equipped_sword->UpdateCollisionParamsCenterPosition(equipped_sword);
		}
		else if ((current_direction == Direction::BACK))
		{
			equipped_sword->SetPosition(this->GetPosition() + Vector2D(-5.0f, 5.0f));
			equipped_sword->UpdateCollisionParamsCenterPosition(equipped_sword);
		}
		break;
	case PlayerState::DAMAGE:
		SetAnimation(AnimType::DAMAGED, ANIMATION_SPPED_DAMAGED);
		break;
	case PlayerState::DEAD:
		break;

	}
}

void Player::OnLeavePlayerState(PlayerState state)
{
	switch (state) {
	case PlayerState::ATTACK:
		// swordを無効化
		equipped_sword->SetActive(false);
	}
}

void Player::UpdateInput()
{
	char _key[256];
	GetHitKeyStateAll(_key);
	for (int i = 0; i < 256; i++)
	{
		if (_key[i] != 0)
		{
			key[i]++;
		}
		else
		{
			key[i] = 0;
		}
	}
}

void Player::UpdateInvincibleTimer()
{
	// 無敵モードならタイマーを動かす。0になったら無敵モードを解除
	if (is_invincible) {
		invincible_timer--;
		if (invincible_timer <= 0) {
			is_invincible = false;
		}
	}
}

void Player::OnHitGroundCollision(float hit_mapchip_position, HitCollisionDirection hit_collsion_direction)
{
	switch (hit_collsion_direction)
	{
	case HitCollisionDirection::BOTTOM:
		position.y = hit_mapchip_position - (body_collision_params.box_extent.y / 2 - 1) - center_dir.y ;		
		UpdateCollisionParamsCenterPosition(this);
		verocity.y = 0.0f;
		current_isground = IsGround::OnGround;
		break;
	case HitCollisionDirection::TOP:
		position.y = hit_mapchip_position + SIZE_CHIP_HEIGHT + (body_collision_params.box_extent.y / 2 - 1) - center_dir.y;
		UpdateCollisionParamsCenterPosition(this);
		break;
	case HitCollisionDirection::RIGHT:
		position.x = hit_mapchip_position  - (body_collision_params.box_extent.x / 2 - 1) - center_dir.x;
		UpdateCollisionParamsCenterPosition(this);
		break;
	case HitCollisionDirection::LEFT:
		position.x = hit_mapchip_position + SIZE_CHIP_WIDTH + (body_collision_params.box_extent.x / 2 - 1) - center_dir.x;
		UpdateCollisionParamsCenterPosition(this);
		break;
	case HitCollisionDirection::NOHIT:
		break;
	}
}

void Player::OnHitObject(class GameObject* opponent_gameobject)
{
	
}

void Player::OnDamaged(int damage, Character* damaged_character)
{
	__super::OnDamaged(damage, damaged_character);
	const float INVINCIBLE_TIMER = 60.0f; // 無敵時間
	const float NOCKBACK_DELTA_POSITION = 20.0f; // ノックバックで動く距離
	// 少しの時間だけ無敵
	SetInvincibleMode(true, INVINCIBLE_TIMER);
	// ダメージステートに変更
	ChangePlayerState(PlayerState::DAMAGE);

	// ノックバック
	position.x -= NOCKBACK_DELTA_POSITION;
	delta_position.x -= NOCKBACK_DELTA_POSITION;
	UpdateCollisionParamsCenterPosition(this);
}
