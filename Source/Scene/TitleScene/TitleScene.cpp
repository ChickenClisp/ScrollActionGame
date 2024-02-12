#include "TitleScene.h"
#include "../../SystemTypes.h"
#include "DxLib.h"
#include "../../Utility/Vector2D.h"
#include "../../Source/GameObject/BackImage/BackImage.h"
#include <string>
#include <fstream>
#include <sstream>

TitleScene::TitleScene()
{
}

void TitleScene::Initialize()
{
	// 親クラスのInitialize()
	__super::Initialize();

	// BackImage
	CreateObject<BackImage>(Vector2D(SCREEN_RESOLUTION_X / 2.0f, SCREEN_RESOLUTION_Y / 2.0f));
}

SceneType TitleScene::Update(float delta_seconds)
{
	// 親クラスのUpdate()
	SceneType result_scene_type = __super::Update(delta_seconds);

	// 入力を受け取る
	UpdateInput();
	// ENTER(RETURN)キーでスタート INGAME_SCENEに遷移
	if (key[KEY_INPUT_RETURN])
	{
		result_scene_type = SceneType::INGAME_SCENE;
	}

	// 現在のシーンタイプを返す
	return result_scene_type;
}

void TitleScene::Draw()
{
	// 親クラスのDraw()
	__super::Draw();

	// Title画面を表示
	DrawExtendString(SCREEN_RESOLUTION_X / 2.0f - 120.0f, SCREEN_RESOLUTION_Y / 2.0f, 2.0f, 2.0f, "2D Platformer", GetColor(255, 255, 255));
	DrawString(SCREEN_RESOLUTION_X / 2.0f - 110.0f, SCREEN_RESOLUTION_Y / 2.0f + 60.0f, "Press Enter     : START", GetColor(255, 255, 255));
}

void TitleScene::Finalize()
{
	// 親クラスのFinalize()
	__super::Finalize();
}
