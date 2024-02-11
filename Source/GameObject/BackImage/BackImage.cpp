#include "BackImage.h"
#include "../../SystemTypes.h"
#include "../../Utility/GraphicResourceManager.h"
#include <DxLib.h>

BackImage::BackImage()
	: graphic_handle_scroll()
{
}

BackImage::~BackImage()
{
}

void BackImage::Initialize()
{
	// 画像の読み込み
	GraphicResourceManager& graphic_resource_manager = GraphicResourceManager::GetInstance();
	graphic_handle_scroll[0] = graphic_resource_manager.LoadGraphicResource(_T("Resources/Images/BackGround/background_layer_1_720_480.png"));
	graphic_handle_scroll[1] = graphic_resource_manager.LoadGraphicResource(_T("Resources/Images/BackGround/background_layer_2_720_480.png"));
	graphic_handle_scroll[2] = graphic_resource_manager.LoadGraphicResource(_T("Resources/Images/BackGround/background_layer_3_720_480.png"));

	// メンバー変数の初期化
	body_collision_params = { Vector2D(), Vector2D(), CollisionObjectType::GROUND, CollisonType::OVERLAP };
}

void BackImage::Update(float delta_seconds)
{
	
}

void BackImage::Draw(const Vector2D& screen_offset)
{
	// 親クラスのDraw()
	__super::Draw(screen_offset);

	// スクリーン座標に変換して背景画像の描画(スクロール)
	int screen_offset_x, screen_offset_y;
	screen_offset.ToInt(screen_offset_x, screen_offset_y);

	int position_integer_per_screensize = screen_offset_x / SCREEN_RESOLUTION_X; // ステージサイズをスクリーンサイズごとに割った配列の要素番号
	
	DrawGraph(SCREEN_RESOLUTION_X * position_integer_per_screensize - screen_offset_x, 0 - screen_offset_y, graphic_handle_scroll[0], true);
	DrawGraph(SCREEN_RESOLUTION_X * (position_integer_per_screensize + 1) - screen_offset_x, 0 - screen_offset_y, graphic_handle_scroll[0], true);
	DrawGraph(SCREEN_RESOLUTION_X * position_integer_per_screensize - screen_offset_x, 0 - screen_offset_y, graphic_handle_scroll[1], true);
	DrawGraph(SCREEN_RESOLUTION_X * (position_integer_per_screensize + 1) - screen_offset_x, 0 - screen_offset_y, graphic_handle_scroll[1], true);
	DrawGraph(SCREEN_RESOLUTION_X * position_integer_per_screensize - screen_offset_x, 0 - screen_offset_y, graphic_handle_scroll[2], true);
	DrawGraph(SCREEN_RESOLUTION_X * (position_integer_per_screensize + 1) - screen_offset_x, 0 - screen_offset_y, graphic_handle_scroll[2], true);
}

void BackImage::Finalize()
{
}
