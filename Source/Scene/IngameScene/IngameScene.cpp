#include "IngameScene.h"
#include "../../SystemTypes.h"
#include "../../GameObject/Character/Player/Player.h"
#include "../../GameObject/BackImage/BackImage.h"
#include "DxLib.h"
#include "../../Utility/Vector2D.h"

IngameScene::IngameScene()
	: player(nullptr)
{
}

void IngameScene::Initialize()
{
	// 親クラスのInitialize()
	__super::Initialize();

	// Playerを生成
	CreateObject<BackImage>(Vector2D(SCREEN_RESOLUTION_X / 2.0f, SCREEN_RESOLUTION_Y / 2.0f));
	player = CreateObject<Player>(Vector2D(SCREEN_RESOLUTION_X / 2.0f, SCREEN_RESOLUTION_Y / 2.0f));
	
}

SceneType IngameScene::Update(float delta_seconds)
{
	// 参考URL https://yttm-work.jp/gmpg/2d_game/2d_game_0002.html
	// 参考URL https://yttm-work.jp/gmpg/2d_game/2d_game_0005.html

	// カメラ座標の更新
	camera_position = player->GetPosition();
	// x軸のステージの内外判定
	if (camera_position.x - float(SCREEN_RESOLUTION_X / 2) <= 0.0f)
	{
		camera_position.x = float(SCREEN_RESOLUTION_X / 2);
	}
	if (camera_position.x + float(SCREEN_RESOLUTION_X / 2) >= 3000.0f) // stage_size = 3000.0f
	{
		camera_position.x = 2000.0f - float(SCREEN_RESOLUTION_X / 2);
	}

	// スクロール用変数(x座標)の更新
	screen_offset.x = camera_position.x - float(SCREEN_RESOLUTION_X / 2);
	printfDx("player: %f\n", player->GetPosition().x);
	printfDx("screen: %f\n", screen_offset.x);

	// 親クラスのUpdate()
	return __super::Update(delta_seconds);

}

void IngameScene::Draw()
{
	// 親クラスのDraw()
	__super::Draw();
}

void IngameScene::Finalize()
{
	// 親クラスのFinalize()
	__super::Finalize();
}