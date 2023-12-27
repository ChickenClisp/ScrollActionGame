#include "Slime.h"

#include "DxLib.h"
#include "../Source/Utility/GraphicResourceManager.h"
#include "../../../SystemTypes.h"
#include "../../Ground/Ground.h"
#include "../Source/Scene/SceneBase.h"
#include "../Source/Scene/IngameScene/IngameScene.h"

Slime::Slime()
	: current_slime_state()
	, current_slime_direction()
	, current_slime_isground()
	, verocity({})
	, key()
{
}

Slime::~Slime()
{
	Finalize();
}

void Slime::Initialize()
{
	__super::Initialize();
	// 画像の読み込み
	GraphicResourceManager& graphic_resource_manager = GraphicResourceManager::GetInstance();
	std::vector<int> out_sprite_handles;

	// IDLE
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/Enemy/Slime/slime_idle.png"), 4, 4, 1, 32, 32, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::IDLE, out_sprite_handles);
	// RUN
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/Enemy/Slime/slime_run.png"), 4, 4, 1, 32, 32, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::RUN, out_sprite_handles);
	// ATTACK
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/Enemy/Slime/slime_attack.png"), 5, 5, 1, 32, 32, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::ATTACK, out_sprite_handles);
	// DEAD
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/Enemy/Slime/slime_die.png"), 4, 4, 1, 32, 32, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::DEAD, out_sprite_handles);


	// Playerメンバ変数の初期化
	current_slime_state = SlimeState::RUN; // SetAnimation()するために異なるステートを宣言
	ChangeSlimeState(SlimeState::IDLE);
	current_slime_direction = SlimeDirection::FRONT;
	current_slime_isground = SlimeIsGround::OnGround;

	center_dir = { 16, 16 };
	body_collision_params = { Vector2D{GetPosition() + center_dir }, Vector2D{32, 32}, CollisionObjectType::ENEMY , 0, CollisonType::BLOCK };
}

void Slime::Update(float delta_seconds)
{
	__super::Update(delta_seconds);
	const float MAX_SPEED = 300.0f;
	const float JUMP_POWER = 800.0f;
	const float GRAVITY = 50.0f;

	prev_position = GetPosition();


	if (current_slime_direction == SlimeDirection::FRONT)
	{
		verocity.x = -30.0f;
	}
	else if (current_slime_direction == SlimeDirection::BACK)
	{
		verocity.x = 30.0f;
	}

	// もしPlayerが索敵範囲内にいたら、攻撃する
	
	//ChangeSlimeState(SlimeState::ATTACK);

	// SlimeStateの遷移条件
	switch (current_slime_state) {
	case SlimeState::IDLE:
		if (abs(verocity.x) > 0)
		{
			ChangeSlimeState(SlimeState::RUN);
		}
		break;
	case SlimeState::RUN:
		if (verocity.x == 0.0f)
		{
			ChangeSlimeState(SlimeState::IDLE);
		}
		break;
	case SlimeState::ATTACK:
		verocity.x = 0.0f;
		// ATTACKのアニメーションが終われば
		if (animation_frame == graphic_handles_map[AnimType::ATTACK].size() - 1)
		{
			ChangeSlimeState(SlimeState::RUN);
		}
		break;
	}

	verocity.y += GRAVITY;

	// 移動ベクトルを求める	
	delta_position = verocity * delta_seconds;
	// 新しいx座標がステージ外であった場合、移動ベクトルを0にする
	if (prev_position.x + delta_position.x < -(128 - (64 + body_collision_params.box_extent.x / 2)) ||
		(prev_position.x + delta_position.x) >(owner_scene->stage_size.x - (128 - (64 - body_collision_params.box_extent.x / 2))))
	{
		delta_position.x = 0.0f;
	}
	// 座標の更新
	SetPosition(prev_position + delta_position);
	UpdateCollisionParams();
}




void Slime::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);

	// 現在のアニメーションを取得
	graphic_handle = graphic_handles_map[animtype][animation_frame];
	// スクリーン座標に変換して描画
	int x, y, screen_offset_x, screen_offset_y;
	GetPosition().ToInt(x, y);
	screen_offset.ToInt(screen_offset_x, screen_offset_y);
	if (current_slime_direction == SlimeDirection::FRONT)
	{
		DrawGraph(x - screen_offset_x, y - screen_offset_y, graphic_handle, true);
	}
	else
	{
		DrawTurnGraph(x - screen_offset_x, y - screen_offset_y, graphic_handle, true);
	}
	// デバッグ用　コリジョンの表示
	DrawBox(x - screen_offset_x, y - screen_offset_y, x - screen_offset_x + 32, y - screen_offset_y + 32, GetColor(0, 0, 255), false);
	DrawBox(body_collision_params.center_position.x - (body_collision_params.box_extent.x / 2 - 1) - screen_offset_x,
		body_collision_params.center_position.y - (body_collision_params.box_extent.y / 2 - 1) - screen_offset_y,
		body_collision_params.center_position.x + (body_collision_params.box_extent.x / 2 - 1) - screen_offset_x,
		body_collision_params.center_position.y + (body_collision_params.box_extent.y / 2 - 1) - screen_offset_y,
		GetColor(0, 255, 0),
		false);
}

void Slime::Finalize()
{
	__super::Finalize();

	// 画像の破棄
	DeleteGraph(graphic_handle);
	graphic_handle = 0;
}

void Slime::ChangeSlimeState(SlimeState new_state)
{
	if (new_state != current_slime_state)
	{
		SetFrameZero();
		OnLeaveSlimeState(current_slime_state);
		current_slime_state = new_state;
		OnEnterSlimeState(current_slime_state);
	}
}

void Slime::OnEnterSlimeState(SlimeState state)
{
	switch (state) {
	case SlimeState::IDLE:
		SetAnimation(AnimType::IDLE, 6);
		break;

	case SlimeState::RUN:
		SetAnimation(AnimType::RUN, 12);
		break;

	case SlimeState::ATTACK:
		SetAnimation(AnimType::ATTACK, 5);
		break;

	case SlimeState::DEAD:
		break;

	}
}

void Slime::OnLeaveSlimeState(SlimeState state)
{
}

void Slime::UpdateCollisionParams()
{
	// コリジョンパラメータの更新
	body_collision_params.center_position = GetPosition() + center_dir;
}

void Slime::OnHitGroundCollision(float hit_mapchip_position, HitCollisionDirection hit_collsion_direction)
{
	switch (hit_collsion_direction)
	{
	case HitCollisionDirection::BOTTOM:
		position.y = hit_mapchip_position - (body_collision_params.box_extent.y / 2 - 1) - center_dir.y;
		UpdateCollisionParams();
		verocity.y = 0.0f;
		current_slime_isground = SlimeIsGround::OnGround;
		break;
	case HitCollisionDirection::TOP:
		position.y = hit_mapchip_position + SIZE_CHIP_HEIGHT + (body_collision_params.box_extent.y / 2 - 1) - center_dir.y;
		UpdateCollisionParams();
		break;
	case HitCollisionDirection::RIGHT:
		position.x = hit_mapchip_position - (body_collision_params.box_extent.x / 2 - 1) - center_dir.x;
		UpdateCollisionParams();
		current_slime_direction = SlimeDirection::FRONT;
		break;
	case HitCollisionDirection::LEFT:
		position.x = hit_mapchip_position + SIZE_CHIP_WIDTH + (body_collision_params.box_extent.x / 2 - 1) - center_dir.x;
		UpdateCollisionParams();
		current_slime_direction = SlimeDirection::BACK;
		break;
	case HitCollisionDirection::NOHIT:
		break;
	}
}