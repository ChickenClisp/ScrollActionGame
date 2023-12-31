#include "SceneManager.h"

#include <cassert>
#include "DxLib.h"

#include "SceneBase.h"
#include "IngameScene/IngameScene.h"

SceneManager::SceneManager()
	: current_scene(nullptr)
{
}

SceneManager::~SceneManager()
{
	Finalize();
}

void SceneManager::Initialize()
{
	// 開始シーンをINGAME_SCENEに設定
	ChangeScene(SceneType::INGAME_SCENE);
}

void SceneManager::Update(float DeltaSeconds)
{
	// シーンの更新
	SceneType result_scene_type = current_scene->Update(DeltaSeconds);

	// 現在とUpdateから受け取ったシーンが別の場合、シーンの遷移を行う
	if (result_scene_type != current_scene->GetSceneType())
	{
		ChangeScene(result_scene_type);
	}

	// 描画
	Draw();
}

void SceneManager::Finalize()
{
	// CurrentSceneの解放
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
		current_scene = nullptr;
	}
}

void SceneManager::Draw()
{
	// 裏画面に描画
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面をクリア
	ClearDrawScreen();

	// シーンの描画
	current_scene->Draw();

	// 画面を更新
	ScreenFlip();
}

void SceneManager::ChangeScene(SceneType new_scene_type)
{
	// シーンの生成
	SceneBase* new_scene = CreateScene(new_scene_type);
	assert(new_scene != nullptr);

	// 現在のシーンの解放
	// 最初のシーン生成時のみCurrentSceneがnullptr
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
	}

	// 新しいシーンの開始
	new_scene->Initialize();
	current_scene = new_scene;
}

SceneBase* SceneManager::CreateScene(SceneType new_scene_type)
{
	switch (new_scene_type)
	{
	case SceneType::INGAME_SCENE:	return new IngameScene();
	default:					return nullptr;
	}
}