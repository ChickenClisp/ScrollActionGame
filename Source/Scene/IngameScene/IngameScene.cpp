#include "IngameScene.h"
#include "../../SystemTypes.h"
#include "../../GameObject/Character/Player/Player.h"
#include "../../GameObject/Character/EnemyBase/Slime/Slime.h"
#include "../../GameObject/BackImage/BackImage.h"
#include "../../GameObject/Ground/Ground.h"
#include "../../GameObject/Sword/Sword.h"
#include "../../GameObject/Goal/Goal.h"
#include "DxLib.h"
#include "../../Utility/Vector2D.h"
#include <string>
#include <fstream>
#include <sstream>

IngameScene::IngameScene()
	: stage_data()
	, player(nullptr)
	, is_goal()
	, player_life()
{
}

void IngameScene::Initialize()
{
	// �e�N���X��Initialize()
	__super::Initialize();

	// �ϐ��̏�����
	is_goal = false;
	player_life = 3;

	// �}�b�v�̓ǂݍ���
	LoadCSV("Resources/stage2.csv", stage_data);
	stage_size.x = (stage_data[0].size() - 1) * SIZE_CHIP_WIDTH; // -1�̗��R�F�E�[�̗񂪂��ׂĂO�̃v���C���[�֎~�G���A�����邽��
	stage_size.y = (stage_data.size() - 2) * SIZE_CHIP_HEIGHT;   // -2�̗��R�F���[2�s�����ׂĂO�̃v���C���[�֎~�G���A�����邽��

	// Object�𐶐�
	InitStage();
	/*
	CreateObject<BackImage>(Vector2D(SCREEN_RESOLUTION_X / 2.0f, SCREEN_RESOLUTION_Y / 2.0f));
	CreateObject<Goal>(Vector2D(500.0f, 320.0f));
	player = CreateObject<Player>(Vector2D(SCREEN_RESOLUTION_X / 8.0f, SCREEN_RESOLUTION_Y * 3.0f / 4.0f));
	CreateObject<Slime>(Vector2D(500.0f, 320.0f));
	CreateObject<Slime>(Vector2D(700.0f, 360.0f));
	CreateObject<Slime>(Vector2D(900.0f, 360.0f));
	CreateObject<Slime>(Vector2D(2700.0f, 340.0f));
	CreateObject<Slime>(Vector2D(4000.0f, 340.0f));
	CreateObject<Slime>(Vector2D(4100.0f, 340.0f));
	class Sword* sword = CreateObject<Sword>(Vector2D(0.0f, 0.0f));
	player->SetSword(sword);
	ground = CreateObject<Ground>(Vector2D(0, 0));
	ground->SetGroundData(stage_data);
	*/
}

SceneType IngameScene::Update(float delta_seconds)
{
	// �e�N���X��Update()
	SceneType result_scene_type = __super::Update(delta_seconds);

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

	// player�`�F�b�N
	if (player->is_active == false)
	{
		OnPlayerDead();
	}
	
	return result_scene_type;
}

void IngameScene::Draw()
{
	// �e�N���X��Draw()
	__super::Draw();

	// �S�[�������ꍇ�AGOAL!!��\��
	if (is_goal == true)
	{
		DrawString(SCREEN_RESOLUTION_X / 2.0f - 30.0f, SCREEN_RESOLUTION_Y / 2.0f, "GOAL!!", GetColor(255, 255, 255));
	}
	// �v���C���[�̎c�@��0�ɂȂ����ꍇ�AGame Over...��\��
	if (player_life == 0)
	{
		DrawString(SCREEN_RESOLUTION_X / 2.0f - 100.0f, SCREEN_RESOLUTION_Y / 2.0f, "Game Over...", GetColor(255, 255, 255));
	}
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

void IngameScene::InitStage()
{
	// Object�𐶐�
	CreateObject<BackImage>(Vector2D(SCREEN_RESOLUTION_X / 2.0f, SCREEN_RESOLUTION_Y / 2.0f));
	CreateObject<Goal>(Vector2D(4300.0f, 205.0f));
	player = CreateObject<Player>(Vector2D(SCREEN_RESOLUTION_X / 8.0f, SCREEN_RESOLUTION_Y * 3.0f / 4.0f));
	CreateObject<Slime>(Vector2D(500.0f, 320.0f));
	CreateObject<Slime>(Vector2D(700.0f, 360.0f));
	CreateObject<Slime>(Vector2D(900.0f, 360.0f));
	CreateObject<Slime>(Vector2D(2700.0f, 340.0f));
	CreateObject<Slime>(Vector2D(4000.0f, 340.0f));
	CreateObject<Slime>(Vector2D(4100.0f, 340.0f));
	class Sword* sword = CreateObject<Sword>(Vector2D(0.0f, 0.0f));
	player->SetSword(sword);
	ground = CreateObject<Ground>(Vector2D(0, 0));
	ground->SetGroundData(stage_data);
}

bool IngameScene::IsFoundPlayer(EnemyBase* enemy_base)
{
	// �v���C���[�����G���[�h�Ȃ���G�ł��Ȃ��Ƃ���
	if (player->GetInvincibleMode())
	{
		return false;
	}
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

void IngameScene::PlayertoEnemyAttackEvent(EnemyBase* enemy_base)
{
	if (enemy_base != nullptr)
	{
		player->ApplyDamage(player->GetAttackPower(), enemy_base);
	}
}

void IngameScene::EnemytoPlayerAttackEvent(EnemyBase* enemy_base)
{
	if (enemy_base != nullptr)
	{
		enemy_base->ApplyDamage(enemy_base->GetAttackPower(), player);
	}
}

void IngameScene::OnPlayerDead()
{
	// �v���C���[�c�@��1���炷
	player_life--;
	// �v���C���[�c�@��0�̏ꍇ
	if (player_life == 0)
	{
		// ���ׂẴI�u�W�F�N�g���폜
		DestroyAllObjects();
		// �V�[���J��
		/*******************
		********************/
	}
	// �v���C���[�c�@��1�ȏ�̏ꍇ
	else
	{
		// �X�e�[�W�̏�����
		DestroyAllObjects();
		InitStage();
	}
}

void IngameScene::OnPlayerGoalReached()
{
	is_goal = true;
}
