#include "IngameScene.h"
#include "../../SystemTypes.h"
#include "../../GameObject/Character/Player/Player.h"
#include "../../GameObject/Character/EnemyBase/Slime/Slime.h"
#include "../../GameObject/BackImage/BackImage.h"
#include "../../GameObject/Ground/Ground.h"
#include "DxLib.h"
#include "../../Utility/Vector2D.h"
#include <string>
#include <fstream>
#include <sstream>

IngameScene::IngameScene()
	: player(nullptr)
	, slime(nullptr)
{
}

void IngameScene::Initialize()
{
	// �e�N���X��Initialize()
	__super::Initialize();

	// �}�b�v�̓ǂݍ���
	std::vector<std::vector<int>> stage_data;
	LoadCSV("Resources/stage2.csv", stage_data);
	stage_size.x = (stage_data[0].size() - 1) * SIZE_CHIP_WIDTH; // -1�̗��R�F�E�[�̗�v�f�����ׂĂO�̃v���C���[�֎~�G���A�����邽��
	stage_size.y = stage_data.size() * SIZE_CHIP_HEIGHT;

	// Object�𐶐�
	CreateObject<BackImage>(Vector2D(SCREEN_RESOLUTION_X / 2.0f, SCREEN_RESOLUTION_Y / 2.0f));
	player = CreateObject<Player>(Vector2D(SCREEN_RESOLUTION_X / 8.0f, SCREEN_RESOLUTION_Y * 3.0f / 4.0f));
	slime = CreateObject<Slime>(Vector2D(500.0f, 320.0f));
	ground = CreateObject<Ground>(Vector2D(0, 0));
	ground->SetGroundData(stage_data);
	// �ړ�����I�u�W�F�N�g��z��Ɋi�[
	move_objects.push_back(player);
	move_objects.push_back(slime);
	
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
	if (camera_position.x + float(SCREEN_RESOLUTION_X / 2) >= float(stage_size.x))
	{
		camera_position.x = float(stage_size.x) - float(SCREEN_RESOLUTION_X / 2);
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

void IngameScene::LoadCSV(const std::string& filename, std::vector<std::vector<int>>& ground_data)
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
		ground_data.emplace_back();
		for (int j = 0; j < strvec.size(); j++)
		{
			ground_data[i].push_back(std::stoi(strvec.at(j)));
		}
	}
}

bool IngameScene::IsFoundPlayer(EnemyBase* enemy_base)
{
	// �v���C���[�ƓG�̋������v�Z
	Vector2D distance = Vector2D((player->GetCollisionParams().center_position.x - enemy_base->GetCollisionParams().center_position.x),
		(player->GetCollisionParams().center_position.y - enemy_base->GetCollisionParams().center_position.y));
	// �T�[�`�͈͓��Ȃ��true�A�����łȂ��Ȃ��false��Ԃ�
	if (distance.Length() < enemy_base->GetSearchRadius())
	{
		return true;
	}
	else
	{
		return false;
	}
}

Direction IngameScene::VectorEnemytoPlayer(EnemyBase* enemy_base)
{
	Vector2D vec = player->GetPosition() - enemy_base->GetPosition();
	if (vec.x > 0.0f) {
		return Direction::BACK;
	}
	else 
	{
		return Direction::FRONT;
	}
}

void IngameScene::AttackEvent(Character* character1, Character* character2)
{
	if (character1 != nullptr && character2 != nullptr)
	{
		character1->ApplyDamage(character1->GetAttackPower(), character2);
	}
}
