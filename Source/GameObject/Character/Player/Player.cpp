#include "Player.h"

#include "DxLib.h"
#include "../Source/Utility/GraphicResourceManager.h"
#include "../../../SystemTypes.h"

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
	// 画像の読み込み
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
	
	// Playerメンバ変数の初期化
	current_player_state = PlayerState::RUN; // SetAnimation()するために異なるステートを宣言
	ChangePlayerState(PlayerState::IDLE);
	current_player_direction = PlayerDirection::FRONT;
	current_player_isground = PlayerIsGround::OnGround;
}

void Player::Update(float delta_seconds)
{
	__super::Update(delta_seconds);
	const float MAX_SPEED = 300.0f;
	const float JUMP_POWER = 1000.0f;
	const float GRAVITY = 50.0f;
	UpdateInput();

	// もしAorDが押されていなかったら、速度を下げる
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
	// もしAが押されていたら、左向きに速度を上げる
	else if (key[KEY_INPUT_A])
	{
		verocity.x -= 10.0f;
		if (abs(verocity.x) > MAX_SPEED)
		{
			verocity.x = -MAX_SPEED;
		}
		current_player_direction = PlayerDirection::BACK;
	}
	// もしDが押されていたら、右向きに速度を上げる
	else if (key[KEY_INPUT_D])
	{
		verocity.x += 10.0f;
		if (abs(verocity.x) > MAX_SPEED)
		{
			verocity.x = MAX_SPEED;
		}
		current_player_direction = PlayerDirection::FRONT;
	}
	// もしSPACEが押されたら、ジャンプ
	if (key[KEY_INPUT_SPACE]==1 && current_player_isground == PlayerIsGround::OnGround)
	{
		verocity.y -= JUMP_POWER;
		current_player_isground = PlayerIsGround::InAir;
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
		// ATTACKのアニメーションが終われば
		if (animation_frame == graphic_handles_map[AnimType::ATTACK].size()-1)
		{
			ChangePlayerState(PlayerState::IDLE);
		}
		break;
	}

	// 座標の更新
	verocity.y += GRAVITY;
	Vector2D delta_position = verocity * delta_seconds;
	SetPosition(GetPosition() + delta_position);

	// 以下、当たり判定実装時に削除
	if (GetPosition().y > SCREEN_RESOLUTION_Y / 2.0f)
	{
		SetPosition(Vector2D(GetPosition().x, SCREEN_RESOLUTION_Y / 2.0f));
		verocity.y = 0.0f;
		current_player_isground = PlayerIsGround::OnGround;
	}
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