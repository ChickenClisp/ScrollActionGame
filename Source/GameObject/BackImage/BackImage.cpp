#include "BackImage.h"
#include "../../SystemTypes.h"
#include <DxLib.h>

BackImage::BackImage()
	: graphic_handle_scroll(0)
{
}

BackImage::~BackImage()
{
}

void BackImage::Initialize()
{
	// 画像の読み込み
	graphic_handle_scroll = LoadGraph(_T("Resources/Images/backimage.png"));
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
	if (position_integer_per_screensize % 2 == 0)
	{
		DrawGraph(SCREEN_RESOLUTION_X * position_integer_per_screensize - screen_offset_x, 0 - screen_offset_y, graphic_handle_scroll, true);
		DrawTurnGraph(SCREEN_RESOLUTION_X * (position_integer_per_screensize + 1) - screen_offset_x, 0 - screen_offset_y, graphic_handle_scroll, true);
	}
	else
	{
		DrawTurnGraph(SCREEN_RESOLUTION_X * position_integer_per_screensize - screen_offset_x, 0 - screen_offset_y, graphic_handle_scroll, true);
		DrawGraph(SCREEN_RESOLUTION_X * (position_integer_per_screensize + 1) - screen_offset_x, 0 - screen_offset_y, graphic_handle_scroll, true);
	}
}

void BackImage::Finalize()
{
}
