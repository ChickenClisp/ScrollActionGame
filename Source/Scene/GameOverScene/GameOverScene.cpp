#include "GameOverScene.h"
#include "../../SystemTypes.h"
#include "DxLib.h"
#include "../../Utility/Vector2D.h"
#include <string>
#include <fstream>
#include <sstream>

GameOverScene::GameOverScene()
{
}

void GameOverScene::Initialize()
{
	// 親クラスのInitialize()
	__super::Initialize();
}

SceneType GameOverScene::Update(float delta_seconds)
{
	// 親クラスのUpdate()
	SceneType result_scene_type = __super::Update(delta_seconds);

	// 入力を受け取る
	UpdateInput();
	// ENTER(RETURN)キーでリトライ INGAME_SCENEに遷移
	if (key[KEY_INPUT_RETURN])
	{
		result_scene_type = SceneType::INGAME_SCENE;
	}
	// BACK_SPACEキーでタイトルへ戻る TITLE_SCENEに遷移
	if (key[KEY_INPUT_BACK])
	{
		result_scene_type = SceneType::TITLE_SCENE;
	}

	// 現在のシーンタイプを返す
	return result_scene_type;
}

void GameOverScene::Draw()
{
	// 親クラスのDraw()
	__super::Draw();

	// Game Over...を表示
	DrawExtendString(SCREEN_RESOLUTION_X / 2.0f - 100.0f, SCREEN_RESOLUTION_Y / 2.0f, 2.0f, 2.0f, "Game Over...", GetColor(255, 255, 255));
	DrawString(SCREEN_RESOLUTION_X / 2.0f - 130.0f, SCREEN_RESOLUTION_Y / 2.0f + 60.0f, "Press Enter     : Retry", GetColor(255, 255, 255));
	DrawString(SCREEN_RESOLUTION_X / 2.0f - 130.0f, SCREEN_RESOLUTION_Y / 2.0f + 90.0f, "Press BackSpace : Return to Title", GetColor(255, 255, 255));

}

void GameOverScene::Finalize()
{
	// 親クラスのFinalize()
	__super::Finalize();
}
