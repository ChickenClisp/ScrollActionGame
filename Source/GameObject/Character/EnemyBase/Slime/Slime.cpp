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
	// �摜�̓ǂݍ���
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


	// �����o�ϐ��̏�����
	current_enemy_state = EnemyState::IDLE; // SetAnimation()���邽�߂ɈقȂ�X�e�[�g��錾
	ChangeEnemyState(EnemyState::RUN);
	current_direction = Direction::FRONT;
	current_isground = IsGround::OnGround;

	center_dir = { 16, 16 };
	body_collision_params = { Vector2D{GetPosition() + center_dir }, Vector2D{32, 32}, CollisionObjectType::ENEMY , CollisonType::BLOCK };
	search_radius = 40.0f;
	hp = 3;
	attack_power = 1;
}

void Slime::Update(float delta_seconds)
{
	__super::Update(delta_seconds);
	const float MAX_SPEED = 300.0f;
	const float GRAVITY = 50.0f;
	

	prev_position = GetPosition();

	if (current_enemy_state == EnemyState::RUN)
	{
		switch (current_direction)
		{
		case Direction::FRONT:
			verocity.x = -30.0f;
			break;
		case Direction::BACK:
			verocity.x = 30.0f;
			break;
		}
	}

	// ����Player�����G�͈͓��ɂ�����A�U������
	IngameScene* in_game_scene = dynamic_cast<IngameScene*>(owner_scene);
	if (in_game_scene->IsFoundPlayer(this))
	{
		// �v���C���[�̕�������
		current_direction = in_game_scene->VectorEnemytoPlayer(this);
		// �X�s�[�h��0��(�U���O�̗���)
		verocity.x = -0.0f;
		// ATTACK�X�e�[�g�ɑJ��
		ChangeEnemyState(EnemyState::ATTACK);
		
		// �U���A�j���[�V�����̃t���[���ɉ����đ傫���ړ�����
		const int ATTACK_FRAME = 3;
		if (animation_frame == ATTACK_FRAME)
		{
			switch (current_direction)
			{
			case Direction::FRONT:
				verocity.x = -60.0f;
				break;
			case Direction::BACK:
				verocity.x = 60.0f;
				break;
			}
		}
	}

	// SlimeState�̑J�ڏ���
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
		// ATTACK�̃A�j���[�V�������I����
		if (animation_frame == graphic_handles_map[AnimType::ATTACK].size() - 1)
		{
			ChangeEnemyState(EnemyState::RUN);
		}
		break;
	}

	verocity.y += GRAVITY;

	// �ړ��x�N�g�������߂�	
	delta_position = verocity * delta_seconds;
	// �V����x���W���X�e�[�W�O�ł������ꍇ�A�ړ��x�N�g����0�ɂ���
	if (prev_position.x + delta_position.x < -(128 - (64 + body_collision_params.box_extent.x / 2)) ||
		(prev_position.x + delta_position.x) >(owner_scene->stage_size.x - (128 - (64 - body_collision_params.box_extent.x / 2))))
	{
		delta_position.x = 0.0f;
	}
	// ���W�̍X�V
	SetPosition(prev_position + delta_position);
	UpdateCollisionParams();
}

void Slime::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);

	// �f�o�b�O�p�@�R���W�����̕\��
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

	// �摜�̔j��
	//DeleteGraph(graphic_handle);
	graphic_handle = 0;
}

void Slime::OnDead()
{
	__super::OnDead();
	owner_scene->DestroyObject(this);
}

void Slime::OnEnterEnemyState(EnemyState state)
{
	switch (state) {
	case EnemyState::IDLE:
		SetAnimation(AnimType::IDLE, 6, true);
		break;

	case EnemyState::RUN:
		SetAnimation(AnimType::RUN, 12, true);
		break;

	case EnemyState::ATTACK:
		SetAnimation(AnimType::ATTACK, 3, false);
		break;

	case EnemyState::DEAD:
		SetAnimation(AnimType::DEAD, 5, false);
		break;

	}
}

void Slime::OnLeaveEnemyState(EnemyState state)
{
}