#include "SampleObject.h"

#include "DxLib.h"

SampleObject::SampleObject()
	: loaded_sprite_handle(0)
{
}

SampleObject::~SampleObject()
{
	Finalize();
}

void SampleObject::Initialize()
{
	__super::Initialize();

	// ‰æ‘œ‚Ì“Ç‚İ‚İ
	loaded_sprite_handle = LoadGraph(_T("Resources/Images/collon_wait.bmp"));
}

void SampleObject::Update(float delta_seconds)
{
	__super::Update(delta_seconds);

	// “®‚©‚·
	Vector2D input_dir;
	if (CheckHitKey(KEY_INPUT_A) == 1)
	{
		input_dir.x = -1;
	}
	else if (CheckHitKey(KEY_INPUT_D) == 1)
	{
		input_dir.x = 1;
	}

	if (CheckHitKey(KEY_INPUT_W) == 1)
	{
		input_dir.y = -1;
	}
	else if (CheckHitKey(KEY_INPUT_S) == 1)
	{
		input_dir.y = 1;
	}
	
	const float MOVEMENT_SPEED = 300.0f;
	Vector2D delta_position = input_dir.Normalize() * MOVEMENT_SPEED * delta_seconds;
	SetPosition(GetPosition() + delta_position);
}

void SampleObject::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);

	// ‰æ‘œ‚Ì•`‰æ
	int x, y;
	GetPosition().ToInt(x, y);
	DrawGraph(x, y, loaded_sprite_handle, true);
}

void SampleObject::Finalize()
{
	__super::Finalize();

	// ‰æ‘œ‚Ì”jŠü
	DeleteGraph(loaded_sprite_handle);
	loaded_sprite_handle = 0;
}