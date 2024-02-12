#include "BigSlime.h"

#include "DxLib.h"
#include "../Source/Utility/GraphicResourceManager.h"
#include "../../../../SystemTypes.h"
#include "../../../Ground/Ground.h"
#include "../Source/Scene/SceneBase.h"
#include "../Source/Scene/IngameScene/IngameScene.h"
#include "../Source/GameObject/Character/Player/Player.h"

BigSlime::BigSlime()
	: is_wait()
	, attack_interval_timer()
{
}

BigSlime::~BigSlime()
{
	Finalize();
}

void BigSlime::Initialize()
{
	__super::Initialize();
	// �摜�̓ǂݍ���
	GraphicResourceManager& graphic_resource_manager = GraphicResourceManager::GetInstance();
	std::vector<int> out_sprite_handles;
	// IDLE
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/Enemy/Slime/bigslime_idle.png"), 4, 4, 1, 64, 64, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::IDLE, out_sprite_handles);
	// RUN
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/Enemy/Slime/bigslime_run.png"), 4, 4, 1, 64, 64, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::RUN, out_sprite_handles);
	// ATTACK
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/Enemy/Slime/bigslime_attack.png"), 5, 5, 1, 64, 64, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::ATTACK, out_sprite_handles);
	// DAMAGED(DAMAGE��IDLE�Ɠ����A�j���[�V�����Ƃ���)
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/Enemy/Slime/bigslime_idle.png"), 4, 4, 1, 64, 64, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::DAMAGED, out_sprite_handles);
	// DEAD
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/Enemy/Slime/bigslime_die.png"), 4, 4, 1, 64, 64, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::DEAD, out_sprite_handles);


	// �����o�ϐ��̏�����
	current_enemy_state = EnemyState::IDLE; // SetAnimation()���邽�߂ɈقȂ�X�e�[�g��錾
	ChangeEnemyState(EnemyState::RUN);
	current_direction = Direction::FRONT;
	current_isground = IsGround::OnGround;
	center_dir = { 32, 32 };
	body_collision_params = { Vector2D{GetPosition() + center_dir }, Vector2D{40, 40}, CollisionObjectType::ENEMY , CollisonType::BLOCK };
	search_radius = 400.0f;
	hp = 8;
	attack_power = 2;

	is_wait = false;
	attack_interval_timer = ATTACK_INTERVAL_TIME;
}

void BigSlime::Update(float delta_seconds)
{
	__super::Update(delta_seconds);

	// ���݂̈ʒu���擾
	prev_position = GetPosition();

	// �^�C�}�[�𓮂���
	UpdateAttackIntervalTimer();

	// EnemyState�̑J�ڏ����̃`�F�b�N
	UpdateCheckConditionChangeEnemyState(current_enemy_state);

	if (!is_wait)
	{
		// �ړ�
		UpdateRun();

		// ���G�ƍU��
		UpdateSearchAndAttack();
	}
	

	// �ړ��x�N�g�������߂�
	verocity.y += GRAVITY;
	delta_position = verocity * delta_seconds;
	// �V����x���W���X�e�[�W�O�ł������ꍇ�A�ړ��x�N�g����0�ɂ���
	if (prev_position.x + delta_position.x <  -(body_collision_params.box_extent.x / 2) ||
		(prev_position.x + delta_position.x) >(owner_scene->stage_size.x - (body_collision_params.box_extent.x * 3 / 2)))
	{
		delta_position.x = 0.0f;
	}
	// �V����y���W���X�e�[�W�O�ł�����(�X�e�[�W����)�ꍇ�A���S�������s��
	if ((prev_position.y + delta_position.y) > owner_scene->stage_size.y)
	{
		OnDead();
	}
	// ���W�̍X�V
	SetPosition(prev_position + delta_position);
	UpdateCollisionParamsCenterPosition(this);
}

void BigSlime::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);

	// �f�o�b�O�p�@�R���W�����̕\��
	int x, y, screen_offset_x, screen_offset_y;
	GetPosition().ToInt(x, y);
	screen_offset.ToInt(screen_offset_x, screen_offset_y);
	DrawBox(x - screen_offset_x, y - screen_offset_y, x - screen_offset_x + 64, y - screen_offset_y + 64, GetColor(0, 0, 255), false);
	DrawBox(body_collision_params.center_position.x - (body_collision_params.box_extent.x / 2 - 1) - screen_offset_x,
		body_collision_params.center_position.y - (body_collision_params.box_extent.y / 2 - 1) - screen_offset_y,
		body_collision_params.center_position.x + (body_collision_params.box_extent.x / 2 - 1) - screen_offset_x,
		body_collision_params.center_position.y + (body_collision_params.box_extent.y / 2 - 1) - screen_offset_y,
		GetColor(0, 255, 0),
		false);
}

void BigSlime::Finalize()
{
	__super::Finalize();

	// �摜�̔j��
	//DeleteGraph(graphic_handle);
	graphic_handle = 0;
}

void BigSlime::OnDamaged(Character* attack_character, Character* damaged_character)
{
	__super::OnDamaged(attack_character, damaged_character);
}

void BigSlime::OnDead()
{
	__super::OnDead();
	is_active = false;
}

void BigSlime::OnEnterEnemyState(EnemyState state)
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

	case EnemyState::DAMAGE:
		SetAnimation(AnimType::DAMAGED, 6, true);
		verocity.x = 0.0f;
		break;

	case EnemyState::DEAD:
		SetAnimation(AnimType::DEAD, 5, false);
		break;

	}
}

void BigSlime::OnLeaveEnemyState(EnemyState state)
{
	switch (state) {
	case EnemyState::ATTACK:
		is_wait = true;
		break;
	}
}

void BigSlime::UpdateCheckConditionChangeEnemyState(EnemyState state)
{
	// SlimeState�̑J�ڏ���
	switch (current_enemy_state) {
	case EnemyState::IDLE:
		break;
	case EnemyState::RUN:
		break;
	case EnemyState::ATTACK:
		// ATTACK�̃A�j���[�V�������I����
		if (animation_frame == graphic_handles_map[AnimType::ATTACK].size() - 1)
		{
			ChangeEnemyState(EnemyState::IDLE);
		}
		break;
	case EnemyState::DAMAGE:
		// DAMAGE�̃A�j���[�V�������I�������AIDLE�ɃX�e�[�g��ύX
		if (animation_frame == graphic_handles_map[AnimType::DAMAGED].size() - 1)
		{
			ChangeEnemyState(EnemyState::RUN);
		}
	case EnemyState::DEAD:
		break;
	}
}

void BigSlime::UpdateRun()
{
	if (current_enemy_state == EnemyState::RUN)
	{
		switch (current_direction)
		{
		case Direction::FRONT:
			verocity.x = -SPEED;
			break;
		case Direction::BACK:
			verocity.x = SPEED;
			break;
		}
	}
}

void BigSlime::UpdateSearchAndAttack()
{
	// ����Player�����G�͈͓��ɂ�����A�U������
	IngameScene* in_game_scene = dynamic_cast<IngameScene*>(owner_scene);
	if (in_game_scene->IsFoundPlayer(this))
	{
		// �v���C���[�̕�������
		current_direction = in_game_scene->VectorEnemytoPlayer(this);
		// �X�s�[�h��0��(�U���O�̗���)
		verocity.x = 0.0f;
		// ATTACK�X�e�[�g�ɑJ��
		ChangeEnemyState(EnemyState::ATTACK);

		// �U���A�j���[�V�����̃t���[���ɉ����đ傫���ړ�����
		const int ATTACK_FRAME = 2;
		if (animation_frame == ATTACK_FRAME)
		{
			switch (current_direction)
			{
			case Direction::FRONT:
				verocity.x = -300.0f;
				break;
			case Direction::BACK:
				verocity.x = 300.0f;
				break;
			}
		}
	}
}

void BigSlime::UpdateAttackIntervalTimer()
{
	// �C���^�[�o���Ȃ�^�C�}�[�𓮂����B0�ɂȂ�����false�ɂ���
	if (is_wait) {
		attack_interval_timer--;
		if (attack_interval_timer <= 0) {
			is_wait = false;
			attack_interval_timer = ATTACK_INTERVAL_TIME;
		}
	}
}