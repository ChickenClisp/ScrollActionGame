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
	// �e�N���X��Initialize()
	__super::Initialize();
}

SceneType GameOverScene::Update(float delta_seconds)
{
	// �e�N���X��Update()
	SceneType result_scene_type = __super::Update(delta_seconds);

	// ���͂��󂯎��
	UpdateInput();
	// ENTER(RETURN)�L�[�Ń��g���C INGAME_SCENE�ɑJ��
	if (key[KEY_INPUT_RETURN])
	{
		result_scene_type = SceneType::INGAME_SCENE;
	}
	// BACK_SPACE�L�[�Ń^�C�g���֖߂� TITLE_SCENE�ɑJ��
	if (key[KEY_INPUT_BACK])
	{
		result_scene_type = SceneType::TITLE_SCENE;
	}

	// ���݂̃V�[���^�C�v��Ԃ�
	return result_scene_type;
}

void GameOverScene::Draw()
{
	// �e�N���X��Draw()
	__super::Draw();

	// Game Over...��\��
	DrawExtendString(SCREEN_RESOLUTION_X / 2.0f - 100.0f, SCREEN_RESOLUTION_Y / 2.0f, 2.0f, 2.0f, "Game Over...", GetColor(255, 255, 255));
	DrawString(SCREEN_RESOLUTION_X / 2.0f - 130.0f, SCREEN_RESOLUTION_Y / 2.0f + 60.0f, "Press Enter     : Retry", GetColor(255, 255, 255));
	DrawString(SCREEN_RESOLUTION_X / 2.0f - 130.0f, SCREEN_RESOLUTION_Y / 2.0f + 90.0f, "Press BackSpace : Return to Title", GetColor(255, 255, 255));

}

void GameOverScene::Finalize()
{
	// �e�N���X��Finalize()
	__super::Finalize();
}
