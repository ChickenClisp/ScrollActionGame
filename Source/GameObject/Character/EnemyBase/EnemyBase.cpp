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

void EnemyBase::Finalize()
{
	__super::Finalize();

	// �摜�̔j��
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
	// �R���W�����p�����[�^�̍X�V
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

void EnemyBase::OnHitObject(GameObject* opponent_gameobject)
{
	// �Փ˃I�u�W�F�N�g��sword�̏ꍇ
	if (opponent_gameobject->GetCollisionParams().collision_object_type == CollisionObjectType::SWORD)
	{
		// sword���L��������Ă����ꍇ
		Sword* sword = dynamic_cast<Sword*>(opponent_gameobject);
		if (sword->GetActive() == true)
		{
			// �v���C���[����G�ւ̍U���C�x���g���s��
			IngameScene* in_game_scene = dynamic_cast<IngameScene*>(owner_scene);
			in_game_scene->PlayertoEnemyAttackEvent(this);
			// sword�𖳌���(�d�����čU�������Ȃ�����)
			sword->SetActive(false);
		}
	}
}
