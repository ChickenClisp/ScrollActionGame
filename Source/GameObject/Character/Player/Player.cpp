#include "Player.h"

#include "DxLib.h"
#include "../Source/Utility/GraphicResourceManager.h"
#include "../../../SystemTypes.h"
#include "../../Ground/Ground.h"

Player::Player()
	: current_player_state()
	, current_player_direction()
	, current_player_isground()
	, verocity({})
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
	// �摜�̓ǂݍ���
	GraphicResourceManager& graphic_resource_manager = GraphicResourceManager::GetInstance();
	std::vector<int> out_sprite_handles;
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
	
	// Player�����o�ϐ��̏�����
	current_player_state = PlayerState::RUN; // SetAnimation()���邽�߂ɈقȂ�X�e�[�g��錾
	ChangePlayerState(PlayerState::IDLE);
	current_player_direction = PlayerDirection::FRONT;
	current_player_isground = PlayerIsGround::OnGround;

	center_dir = { 64, 80 };
	body_collision_params = { Vector2D{GetPosition() + center_dir }, Vector2D{36, 56}, CollisionObjectType::PLAYER , 0, CollisonType::BLOCK };
}

void Player::Update(float delta_seconds)
{
	__super::Update(delta_seconds);
	const float MAX_SPEED = 300.0f;
	const float JUMP_POWER = 400.0f;
	const float GRAVITY = 18.0f;
	UpdateInput();

	prev_position = GetPosition();

	// ����AorD��������Ă��Ȃ�������A���x��������
	if (!key[KEY_INPUT_A] && !key[KEY_INPUT_D])
	{
		if (verocity.x < 0)
		{
			verocity.x += 20.0f;
		}
		else if(verocity.x > 0)
		{
			verocity.x -= 20.0f;
		}

		if (abs(verocity.x) < 20.0f)
		{
			verocity.x = 0.0f;
		}
	}
	// ����A��������Ă�����A�������ɑ��x���グ��
	else if (key[KEY_INPUT_A])
	{
		verocity.x -= 10.0f;
		if (abs(verocity.x) > MAX_SPEED)
		{
			verocity.x = -MAX_SPEED;
		}
		current_player_direction = PlayerDirection::BACK;
	}
	// ����D��������Ă�����A�E�����ɑ��x���グ��
	else if (key[KEY_INPUT_D])
	{
		verocity.x += 10.0f;
		if (abs(verocity.x) > MAX_SPEED)
		{
			verocity.x = MAX_SPEED;
		}
		current_player_direction = PlayerDirection::FRONT;
	}
	// ����SPACE�������ꂽ��A�W�����v
	if (key[KEY_INPUT_SPACE]==1 && current_player_isground == PlayerIsGround::OnGround)
	{
		verocity.y -= JUMP_POWER;
		current_player_isground = PlayerIsGround::InAir;
		ChangePlayerState(PlayerState::JUMP);
	}
	// ����E��������Ă���A�U��
	if (key[KEY_INPUT_E] == 1)
	{
		ChangePlayerState(PlayerState::ATTACK);
	}
	
	// PlayerState�̑J�ڏ���
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
		// JUMP�̃A�j���[�V�������I����
		if (animation_frame == graphic_handles_map[AnimType::JUMP].size() - 1)
		{
			ChangePlayerState(PlayerState::IDLE);
		}
		break;
	case PlayerState::ATTACK:
		// ATTACK�̃A�j���[�V�������I����
		if (animation_frame == graphic_handles_map[AnimType::ATTACK].size()-1)
		{
			ChangePlayerState(PlayerState::IDLE);
		}
		break;
	}

	// ���W�̍X�V
	verocity.y += GRAVITY;
	delta_position = verocity * delta_seconds;
	SetPosition(prev_position + delta_position);
	UpdateCollisionParams();
}




void Player::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);

	// ���݂̃A�j���[�V�������擾
	graphic_handle = graphic_handles_map[animtype][animation_frame];
	// �X�N���[�����W�ɕϊ����ĕ`��
	int x, y, screen_offset_x, screen_offset_y;
	GetPosition().ToInt(x, y);
	screen_offset.ToInt(screen_offset_x, screen_offset_y);
	if (current_player_direction == PlayerDirection::FRONT)
	{
		printfDx("x:%d, y:%d\n", x - screen_offset_x, y - screen_offset_y);
		DrawGraph(x - screen_offset_x, y - screen_offset_y, graphic_handle, true);
	}
	else
	{
		DrawTurnGraph(x - screen_offset_x, y - screen_offset_y, graphic_handle, true);
	}
	// �f�o�b�O�p�@�R���W�����̕\��
	DrawBox(x - screen_offset_x, y - screen_offset_y, x - screen_offset_x + 128, y - screen_offset_y + 128, GetColor(0, 0, 255), false);
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

	// �摜�̔j��
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
	switch (state) {
	case PlayerState::IDLE:
		SetAnimation(AnimType::IDLE, 6);
		break;

	case PlayerState::RUN:
		SetAnimation(AnimType::RUN, 12);
		break;
	
	case PlayerState::JUMP:
		SetAnimation(AnimType::JUMP, 10);
		break;

	case PlayerState::ATTACK:
		SetAnimation(AnimType::ATTACK, 5);
		break;
	case PlayerState::DAMAGE:
		SetAnimation(AnimType::DAMAGED, 10);
		break;
	case PlayerState::DEAD:
		break;
		
	}
}

void Player::OnLeavePlayerState(PlayerState state)
{
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

void Player::UpdateCollisionParams()
{
	// �R���W�����p�����[�^�̍X�V
	body_collision_params.center_position = GetPosition() + center_dir;
}

void Player::OnHitGroundCollision(float hit_mapchip_position, HitCollisionDirection hit_collsion_direction)
{
	switch (hit_collsion_direction)
	{
	case HitCollisionDirection::BOTTOM:
		position.y = hit_mapchip_position - (body_collision_params.box_extent.y / 2 - 1) - center_dir.y ;		
		UpdateCollisionParams();
		verocity.y = 0.0f;
		current_player_isground = PlayerIsGround::OnGround;
		printfDx("bottom ");
		break;
	case HitCollisionDirection::TOP:
		position.y = hit_mapchip_position + SIZE_CHIP_HEIGHT + (body_collision_params.box_extent.y / 2 - 1) - center_dir.y;
		UpdateCollisionParams();
		printfDx("top ");
		break;
	case HitCollisionDirection::RIGHT:
		position.x = hit_mapchip_position  - (body_collision_params.box_extent.x / 2 - 1) - center_dir.x;
		UpdateCollisionParams();
		printfDx("right ");
		break;
	case HitCollisionDirection::LEFT:
		position.x = hit_mapchip_position + SIZE_CHIP_WIDTH + (body_collision_params.box_extent.x / 2 - 1) - center_dir.x;
		UpdateCollisionParams();
		printfDx("left");
		break;
	case HitCollisionDirection::NOHIT:
		break;
	}
	printfDx("\n");
	
}
