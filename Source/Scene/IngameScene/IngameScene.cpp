#include "IngameScene.h"
#include "../../SystemTypes.h"
#include "../../GameObject/Character/Player/Player.h"
#include "../../GameObject/BackImage/BackImage.h"
#include "DxLib.h"
#include "../../Utility/Vector2D.h"

IngameScene::IngameScene()
	: player(nullptr)
	, stage_size(0)
{
}

void IngameScene::Initialize()
{
	// �e�N���X��Initialize()
	__super::Initialize();

	stage_size = 3000;

	// Player�𐶐�
	CreateObject<BackImage>(Vector2D(SCREEN_RESOLUTION_X / 2.0f, SCREEN_RESOLUTION_Y / 2.0f));
	player = CreateObject<Player>(Vector2D(SCREEN_RESOLUTION_X / 2.0f, SCREEN_RESOLUTION_Y / 2.0f));
	
}

SceneType IngameScene::Update(float delta_seconds)
{
	// �Q�lURL https://yttm-work.jp/gmpg/2d_game/2d_game_0002.html
	// �Q�lURL https://yttm-work.jp/gmpg/2d_game/2d_game_0005.html

	// �J�������W�̍X�V
	camera_position = player->GetPosition();
	// x���̃X�e�[�W�̓��O����
	if (camera_position.x - float(SCREEN_RESOLUTION_X / 2) <= 0.0f)
	{
		camera_position.x = float(SCREEN_RESOLUTION_X / 2);
	}
	if (camera_position.x + float(SCREEN_RESOLUTION_X / 2) >= float(stage_size))
	{
		camera_position.x = float(stage_size) - float(SCREEN_RESOLUTION_X / 2);
	}

	// �X�N���[���p�ϐ�(x���W)�̍X�V
	screen_offset.x = camera_position.x - float(SCREEN_RESOLUTION_X / 2);

	// �e�N���X��Update()
	return __super::Update(delta_seconds);

}

void IngameScene::Draw()
{
	// �e�N���X��Draw()
	__super::Draw();
}

void IngameScene::Finalize()
{
	// �e�N���X��Finalize()
	__super::Finalize();
}