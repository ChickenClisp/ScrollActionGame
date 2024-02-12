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
	// �e�N���X��Initialize()
	__super::Initialize();

	// BackImage
	CreateObject<BackImage>(Vector2D(SCREEN_RESOLUTION_X / 2.0f, SCREEN_RESOLUTION_Y / 2.0f));
}

SceneType TitleScene::Update(float delta_seconds)
{
	// �e�N���X��Update()
	SceneType result_scene_type = __super::Update(delta_seconds);

	// ���͂��󂯎��
	UpdateInput();
	// ENTER(RETURN)�L�[�ŃX�^�[�g INGAME_SCENE�ɑJ��
	if (key[KEY_INPUT_RETURN])
	{
		result_scene_type = SceneType::INGAME_SCENE;
	}

	// ���݂̃V�[���^�C�v��Ԃ�
	return result_scene_type;
}

void TitleScene::Draw()
{
	// �e�N���X��Draw()
	__super::Draw();

	// Title��ʂ�\��
	DrawExtendString(SCREEN_RESOLUTION_X / 2.0f - 120.0f, SCREEN_RESOLUTION_Y / 2.0f, 2.0f, 2.0f, "2D Platformer", GetColor(255, 255, 255));
	DrawString(SCREEN_RESOLUTION_X / 2.0f - 110.0f, SCREEN_RESOLUTION_Y / 2.0f + 60.0f, "Press Enter     : START", GetColor(255, 255, 255));
}

void TitleScene::Finalize()
{
	// �e�N���X��Finalize()
	__super::Finalize();
}
