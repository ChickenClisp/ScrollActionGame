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
}

void EnemyBase::Finalize()
{
	__super::Finalize();

	// 画像の破棄
	DeleteGraph(graphic_handle);
	graphic_handle = 0;
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

void EnemyBase::UpdateCollisionParams()
{
	// コリジョンパラメータの更新
	body_collision_params.center_position = GetPosition() + center_dir;
}

void EnemyBase::OnHitGroundCollision(float hit_mapchip_position, HitCollisionDirection hit_collsion_direction)
{
	switch (hit_collsion_direction)
	{
	case HitCollisionDirection::BOTTOM:
		position.y = hit_mapchip_position - (body_collision_params.box_extent.y / 2 - 1) - center_dir.y;
		UpdateCollisionParams();
		verocity.y = 0.0f;
		current_isground = IsGround::OnGround;
		break;
	case HitCollisionDirection::TOP:
		position.y = hit_mapchip_position + SIZE_CHIP_HEIGHT + (body_collision_params.box_extent.y / 2 - 1) - center_dir.y;
		UpdateCollisionParams();
		break;
	case HitCollisionDirection::RIGHT:
		position.x = hit_mapchip_position - (body_collision_params.box_extent.x / 2 - 1) - center_dir.x;
		UpdateCollisionParams();
		current_direction = Direction::FRONT;
		break;
	case HitCollisionDirection::LEFT:
		position.x = hit_mapchip_position + SIZE_CHIP_WIDTH + (body_collision_params.box_extent.x / 2 - 1) - center_dir.x;
		UpdateCollisionParams();
		current_direction = Direction::BACK;
		break;
	case HitCollisionDirection::NOHIT:
		break;
	}
}