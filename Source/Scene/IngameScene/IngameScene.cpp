#include "IngameScene.h"
#include "../../SystemTypes.h"
#include "../../GameObject/Character/Player/Player.h"
#include "../../GameObject/BackImage/BackImage.h"
#include "../../GameObject/Ground/Ground.h"
#include "DxLib.h"
#include "../../Utility/Vector2D.h"
#include <string>
#include <fstream>
#include <sstream>

IngameScene::IngameScene()
	: player(nullptr)
	, ground(nullptr)
	, stage_size(0)
{
}

void IngameScene::Initialize()
{
	// �e�N���X��Initialize()
	__super::Initialize();

	stage_size = 3000;

	// Object�𐶐�
	CreateObject<BackImage>(Vector2D(SCREEN_RESOLUTION_X / 2.0f, SCREEN_RESOLUTION_Y / 2.0f));
	player = CreateObject<Player>(Vector2D(SCREEN_RESOLUTION_X / 2.0f, SCREEN_RESOLUTION_Y / 2.0f));
	ground = CreateObject<Ground>(Vector2D(0, 0));
	
	// �}�b�v�̓ǂݍ���
	std::vector<std::vector<int>> stage_data = ground->GetGroundData();
	LoadCSV("Resources/stage1.csv", stage_data);
	ground->SetGroundData(stage_data);
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

void IngameScene::LoadCSV(const std::string& filename, std::vector<std::vector<int>>& stage_data)
{
	// �X�e�[�W����csv�t�@�C������擾
	std::ifstream ifs(filename);
	std::string line;
	for (int i = 0; getline(ifs, line); i++)
	{
		// ","�ŕ�����𕪊�����
		std::istringstream stream(line);
		std::string field;
		std::vector<std::string> strvec;
		while (getline(stream, field, ',')) {
			strvec.push_back(field);
		}
		// stage_data��int�^�Ŋi�[
		stage_data.emplace_back();
		for (int j = 0; j < strvec.size(); j++)
		{
			stage_data[i].push_back(std::stoi(strvec.at(j)));
		}
	}
}
