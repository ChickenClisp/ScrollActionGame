#include "Goal.h"
#include "DxLib.h"
#include "../Source/Utility/GraphicResourceManager.h"

Goal::Goal()
{
}

Goal::~Goal()
{
}

void Goal::Initialize()
{
	__super::Initialize();
	center_dir = Vector2D{ 15, 30 };
	body_collision_params = { Vector2D{GetPosition() + center_dir }, Vector2D{30, 60}, CollisionObjectType::ITEM , CollisonType::OVERLAP };
	
	// 画像の読み込み
	GraphicResourceManager& graphic_resource_manager = GraphicResourceManager::GetInstance();
	std::vector<int> out_sprite_handles;
	// IDLE
	graphic_resource_manager.LoadDivGraphicResource(_T("Resources/Images/Goal/goalflag.png"), 5, 5, 1, 60, 60, out_sprite_handles);
	graphic_handles_map.emplace(AnimType::IDLE, out_sprite_handles);

	// アニメーションのセット
	SetAnimation(AnimType::IDLE, 10, true);
}

void Goal::Update(float delta_seconds)
{
	__super::Update(delta_seconds);
	UpdateAnimation();
}

void Goal::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);
	// 現在のアニメーションを取得
	graphic_handle = graphic_handles_map[animtype][animation_frame];
	// スクリーン座標に変換して描画
	int x, y, screen_offset_x, screen_offset_y;
	GetPosition().ToInt(x, y);
	screen_offset.ToInt(screen_offset_x, screen_offset_y);
	DrawGraph(x - screen_offset_x, y - screen_offset_y, graphic_handle, true);

	// デバッグ用　コリジョンの表示
		DrawBox(body_collision_params.center_position.x - (body_collision_params.box_extent.x / 2 - 1) - screen_offset_x,
		body_collision_params.center_position.y - (body_collision_params.box_extent.y / 2 - 1) - screen_offset_y,
		body_collision_params.center_position.x + (body_collision_params.box_extent.x / 2 - 1) - screen_offset_x,
		body_collision_params.center_position.y + (body_collision_params.box_extent.y / 2 - 1) - screen_offset_y,
		GetColor(0, 255, 0),
		false);
}

void Goal::Finalize()
{
	__super::Finalize();

	// 画像の破棄
	//DeleteGraph(graphic_handle);
	graphic_handle = 0;
}