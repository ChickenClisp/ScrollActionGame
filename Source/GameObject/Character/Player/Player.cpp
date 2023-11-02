#include "Player.h"

#include "DxLib.h"
#include "../Source/Utility/GraphicResourceManager.h"

Player::Player()
	: current_player_state(PlayerState::IDLE)
	, current_player_direction(PlayerDirection::FRONT)
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

	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/collon_wait_a.bmp"), 4, 4, 1, 128, 128, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::IDLE, out_sprite_handles);
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/collon_run8.bmp"), 8, 4, 2, 128, 128, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::RUN, out_sprite_handles);

	//LoadDivGraph(PLAYER_JUMP.filename, PLAYER_JUMP.all_num, PLAYER_JUMP.num_x, PLAYER_JUMP.num_y, PLAYER_JUMP.size_x, PLAYER_JUMP.size_y, hundle_buf_jump);
	//LoadDivGraph(PLAYER_ATTACK.filename, PLAYER_ATTACK.all_num, PLAYER_ATTACK.num_x, PLAYER_ATTACK.num_y, PLAYER_ATTACK.size_x, PLAYER_ATTACK.size_y, hundle_buf_attack);
	//LoadDivGraph(PLAYER_DAMAGE.filename, PLAYER_DAMAGE.all_num, PLAYER_DAMAGE.num_x, PLAYER_DAMAGE.num_y, PLAYER_DAMAGE.size_x, PLAYER_DAMAGE.size_y, hundle_buf_damage);

}

void Player::Update(float delta_seconds)
{
	__super::Update(delta_seconds);

	// 動かす
	Vector2D input_dir;
	if (CheckHitKey(KEY_INPUT_A) == 1)
	{
		input_dir.x = -1;
		current_player_direction = PlayerDirection::BACK;
		ChangePlayerState(PlayerState::RUN);
	}
	else if (CheckHitKey(KEY_INPUT_D) == 1)
	{
		input_dir.x = 1;
		current_player_direction = PlayerDirection::FRONT;
		ChangePlayerState(PlayerState::RUN);
	}
	else
	{
		ChangePlayerState(PlayerState::IDLE);
	}

	/*
	if (CheckHitKey(KEY_INPUT_W) == 1)
	{
		input_dir.y = -1;
	}
	else if (CheckHitKey(KEY_INPUT_S) == 1)
	{
		input_dir.y = 1;
	}
	*/
	/*
	if (++frame_adjust > frame_adjust_max)
	{
		frame++;
		frame_adjust = 0;
		frame %= frame_max;
	}
	*/

	const float MOVEMENT_SPEED = 300.0f;
	Vector2D delta_position = input_dir.Normalize() * MOVEMENT_SPEED * delta_seconds;
	SetPosition(GetPosition() + delta_position);
}

void Player::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);

	// スクリーン座標に変換して描画
	int x, y, screen_offset_x, screen_offset_y;
	GetPosition().ToInt(x, y);
	screen_offset.ToInt(screen_offset_x, screen_offset_y);
	if (current_player_direction == PlayerDirection::FRONT)
	{
		DrawGraph(x - screen_offset_x, y - screen_offset_y, graphic_handle, true);
	}
	else
	{
		DrawTurnGraph(x - screen_offset_x, y - screen_offset_y, graphic_handle, true);
	}
	
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
	}
	OnLeavePlayerState(current_player_state);
	current_player_state = new_state;
	OnEnterPlayerState(current_player_state);
	
}

void Player::OnEnterPlayerState(PlayerState state)
{
	switch (state) {
	case PlayerState::IDLE:
		ChangeAnimation(AnimType::IDLE, 10);
		break;

	case PlayerState::RUN:
		ChangeAnimation(AnimType::RUN, 5);
		break;
	/*
	case PlayerState::JUMP:
		graphic_handle = hundle_buf_jump[frame];
		frame_max = PLAYER_JUMP.all_num;
		frame_adjust_max = PLAYER_JUMP.frame_num;
		break;

	case PlayerState::ATTACK:
		graphic_handle = hundle_buf_attack[frame];
		frame_max = PLAYER_ATTACK.all_num;
		frame_adjust_max = PLAYER_ATTACK.frame_num;
		break;
	case PlayerState::DAMAGE:
		break;
	case PlayerState::DEAD:
		break;
		*/
	}
}

void Player::OnLeavePlayerState(PlayerState state)
{
}
