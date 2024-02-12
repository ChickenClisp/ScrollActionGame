#include "Sword.h"
#include "DxLib.h"

Sword::Sword()
{
}

Sword::~Sword()
{
}

void Sword::Initialize()
{
	__super::Initialize();
	is_active = false;
	const Vector2D ATTACK_BOX_EXTENT = Vector2D{ 25.0f, 40.0f };
	body_collision_params = {GetPosition(), ATTACK_BOX_EXTENT, CollisionObjectType::SWORD, CollisonType::OVERLAP };
	center_dir = Vector2D(ATTACK_BOX_EXTENT / 2);
}

void Sword::Update(float delta_seconds)
{
	__super::Update(delta_seconds);
}

void Sword::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);
	
	// スクリーン座標に変換して描画
	int screen_offset_x, screen_offset_y;
	screen_offset.ToInt(screen_offset_x, screen_offset_y);
	// デバッグ用　コリジョンの表示
	DrawBox(body_collision_params.center_position.x - (body_collision_params.box_extent.x / 2 - 1) - screen_offset_x,
		body_collision_params.center_position.y - (body_collision_params.box_extent.y / 2 - 1) - screen_offset_y,
		body_collision_params.center_position.x + (body_collision_params.box_extent.x / 2 - 1) - screen_offset_x,
		body_collision_params.center_position.y + (body_collision_params.box_extent.y / 2 - 1) - screen_offset_y,
		GetColor(0, 255, 0),
		false);
	
}

void Sword::Finalize()
{
	__super::Finalize();
}
