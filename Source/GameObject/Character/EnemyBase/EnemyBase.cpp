#include "EnemyBase.h"

#include "DxLib.h"
#include "../Source/Utility/GraphicResourceManager.h"
#include "../../../SystemTypes.h"
#include "../../Ground/Ground.h"
#include "../Source/Scene/SceneBase.h"
#include "../Source/Scene/IngameScene/IngameScene.h"
#include "../Source/GameObject/Character/Player/Player.h"

EnemyBase::EnemyBase()
	: current_enemy_state()
	, search_radius()
{
}

EnemyBase::~EnemyBase()
{
	Finalize();
}

void EnemyBase::Initialize()
{
	__super::Initialize();
}

void EnemyBase::Update(float delta_seconds)
{
	__super::Update(delta_seconds);
}


void EnemyBase::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);
	// 現在のアニメーションを取得
	graphic_handle = graphic_handles_map[animtype][animation_frame];
	// スクリーン座標に変換して描画
	int x, y, screen_offset_x, screen_offset_y;
	GetPosition().ToInt(x, y);
	screen_offset.ToInt(screen_offset_x, screen_offset_y);
	//　前を向いている(x座標正方向)場合はDrawGraph
	if (current_direction == Direction::FRONT)
	{
		// 攻撃を受けた場合、赤く描画する
		if (current_enemy_state == EnemyState::DAMAGE)
		{
			SetDrawBright(255, 0, 0);
			DrawGraph(x - screen_offset_x, y - screen_offset_y, graphic_handle, true);
			SetDrawBright(255, 255, 255);
		}
		else
		{
			DrawGraph(x - screen_offset_x, y - screen_offset_y, graphic_handle, true);
		}
	}
	//　後ろを向いている(x座標負方向)場合はDrawTurnGraph
	else
	{
		// 攻撃を受けた場合、赤く描画する
		if (current_enemy_state == EnemyState::DAMAGE)
		{
			SetDrawBright(255, 0, 0);
			DrawTurnGraph(x - screen_offset_x, y - screen_offset_y, graphic_handle, true);
			SetDrawBright(255, 255, 255);
		}
		else
		{
			DrawTurnGraph(x - screen_offset_x, y - screen_offset_y, graphic_handle, true);
		}

	}
}

void EnemyBase::Finalize()
{
	__super::Finalize();
}

void EnemyBase::ChangeEnemyState(EnemyState new_state)
{
	if (new_state != current_enemy_state)
	{
		SetFrameZero();
		OnLeaveEnemyState(current_enemy_state);
		current_enemy_state = new_state;
		OnEnterEnemyState(current_enemy_state);
	}
}

void EnemyBase::OnEnterEnemyState(EnemyState state)
{ 
}

void EnemyBase::OnLeaveEnemyState(EnemyState state)
{
}

void EnemyBase::OnDamaged(Character* attack_character, Character* damaged_character)
{
	__super::OnDamaged(attack_character, damaged_character);
	// ダメージステートに変更
	ChangeEnemyState(EnemyState::DAMAGE);
}

void EnemyBase::OnDead()
{
	__super::OnDead();
}

void EnemyBase::OnHitGroundCollision(float hit_mapchip_position, HitCollisionDirection hit_collsion_direction)
{
	switch (hit_collsion_direction)
	{
	case HitCollisionDirection::BOTTOM:
		position.y = hit_mapchip_position - (body_collision_params.box_extent.y / 2 - 1) - center_dir.y;
		UpdateCollisionParamsCenterPosition(this);
		verocity.y = 0.0f;
		current_isground = IsGround::OnGround;
		break;
	case HitCollisionDirection::TOP:
		position.y = hit_mapchip_position + SIZE_CHIP_HEIGHT + (body_collision_params.box_extent.y / 2 - 1) - center_dir.y;
		UpdateCollisionParamsCenterPosition(this);
		break;
	case HitCollisionDirection::RIGHT:
		position.x = hit_mapchip_position - (body_collision_params.box_extent.x / 2 - 1) - center_dir.x;
		UpdateCollisionParamsCenterPosition(this);
		current_direction = Direction::FRONT;
		break;
	case HitCollisionDirection::LEFT:
		position.x = hit_mapchip_position + SIZE_CHIP_WIDTH + (body_collision_params.box_extent.x / 2 - 1) - center_dir.x;
		UpdateCollisionParamsCenterPosition(this);
		current_direction = Direction::BACK;
		break;
	case HitCollisionDirection::NOHIT:
		break;
	}
}

void EnemyBase::OnHitObject(GameObject* opponent_gameobject)
{
	// 衝突オブジェクトがswordの場合
	if (opponent_gameobject->GetCollisionParams().collision_object_type == CollisionObjectType::SWORD)
	{
		// swordが有効かされていた場合
		Sword* sword = dynamic_cast<Sword*>(opponent_gameobject);
		if (sword->GetActive() == true)
		{
			// プレイヤーから敵への攻撃イベントを行う
			IngameScene* in_game_scene = dynamic_cast<IngameScene*>(owner_scene);
			in_game_scene->PlayertoEnemyAttackEvent(this);
			// swordを無効化(重複して攻撃させないため)
			sword->SetActive(false);
		}
	}
}
