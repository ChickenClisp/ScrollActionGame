#include "Slime.h"

#include "DxLib.h"
#include "../Source/Utility/GraphicResourceManager.h"
#include "../../../../SystemTypes.h"
#include "../../../Ground/Ground.h"
#include "../Source/Scene/SceneBase.h"
#include "../Source/Scene/IngameScene/IngameScene.h"
#include "../Source/GameObject/Character/Player/Player.h"

Slime::Slime()
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


	// メンバ変数の初期化
	current_enemy_state = EnemyState::RUN; // SetAnimation()するために異なるステートを宣言
	ChangeEnemyState(EnemyState::IDLE);
	current_direction = Direction::FRONT;
	current_isground = IsGround::OnGround;

	center_dir = { 16, 16 };
	body_collision_params = { Vector2D{GetPosition() + center_dir }, Vector2D{32, 32}, CollisionObjectType::ENEMY , 0, CollisonType::BLOCK };
	search_radius = 40.0f;
	hp = 1;
	attack_power = 1;
}

void Slime::Update(float delta_seconds)
{
	__super::Update(delta_seconds);
	const float MAX_SPEED = 300.0f;
	const float GRAVITY = 50.0f;

	prev_position = GetPosition();


	if (current_direction == Direction::FRONT)
	{
		verocity.x = -30.0f;
	}
	else if (current_direction == Direction::BACK)
	{
		verocity.x = 30.0f;
	}

	// もしPlayerが索敵範囲内にいたら、攻撃する
	IngameScene* in_game_scene = dynamic_cast<IngameScene*>(owner_scene);
	if (in_game_scene->IsFoundPlayer(this))
	{
		// 前進して攻撃する
		if (current_direction == Direction::FRONT)
		{
			verocity.x -= 20.0f;
		}
		else
		{
			verocity.x += 20.0f;
		}
		ChangeEnemyState(EnemyState::ATTACK);
		current_direction = in_game_scene->VectorEnemytoPlayer(this);
	}

	// SlimeStateの遷移条件
	switch (current_enemy_state) {
	case EnemyState::IDLE:
		if (abs(verocity.x) > 0)
		{
			ChangeEnemyState(EnemyState::RUN);
		}
		break;
	case EnemyState::RUN:
		if (verocity.x == 0.0f)
		{
			ChangeEnemyState(EnemyState::IDLE);
		}
		break;
	case EnemyState::ATTACK:
		// ATTACKのアニメーションが終われば
		if (animation_frame == graphic_handles_map[AnimType::ATTACK].size() - 1)
		{
			ChangeEnemyState(EnemyState::RUN);
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

	// デバッグ用　コリジョンの表示
	int x, y, screen_offset_x, screen_offset_y;
	GetPosition().ToInt(x, y);
	screen_offset.ToInt(screen_offset_x, screen_offset_y);
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

void Slime::OnEnterEnemyState(EnemyState state)
{
	switch (state) {
	case EnemyState::IDLE:
		SetAnimation(AnimType::IDLE, 6);
		break;

	case EnemyState::RUN:
		SetAnimation(AnimType::RUN, 12);
		break;

	case EnemyState::ATTACK:
		SetAnimation(AnimType::ATTACK, 5);
		break;

	case EnemyState::DEAD:
		break;

	}
}

void Slime::OnLeaveEnemyState(EnemyState state)
{
}