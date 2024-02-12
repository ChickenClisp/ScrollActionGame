#include "IngameScene.h"
#include "../../SystemTypes.h"
#include "../../GameObject/Character/Player/Player.h"
#include "../../GameObject/Character/EnemyBase/Slime/Slime.h"
#include "../../GameObject/Character/EnemyBase/BigSlime/BigSlime.h"
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
	, enemy_list()
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
	stage_size.y = (stage_data.size() - 3) * SIZE_CHIP_HEIGHT;   // -3�̗��R�F���[3�s�����ׂĂO�̃v���C���[�֎~�G���A�����邽��

	InitStage();
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

	// player�̐����`�F�b�N
	if (player->GetActive() == false)
	{
		result_scene_type = OnPlayerDead();
	}
	// enemy�̐����`�F�b�N
	for (auto &enemy : enemy_list)
	{
		if (enemy->GetActive() == false)
			DestroyObject(enemy);
	}

	// ���݂̃V�[���^�C�v��Ԃ�
	return result_scene_type;
}

void IngameScene::Draw()
{
	// �e�N���X��Draw()
	__super::Draw();

	std::string string_player_life = "LEFT: " + std::to_string(player_life);
	DrawStringF(20, 10, string_player_life.c_str(), GetColor(255, 255, 255));

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
	// BackImage
	CreateObject<BackImage>(Vector2D(SCREEN_RESOLUTION_X / 2.0f, SCREEN_RESOLUTION_Y / 2.0f));
	// Goal
	CreateObject<Goal>(Vector2D(4300.0f, 205.0f));
	// Player
	player = CreateObject<Player>(Vector2D(90.0f, 360.0f));
	// Sword
	class Sword* sword = CreateObject<Sword>(Vector2D());
	player->SetSword(sword);
	// Enemy
	////// Slime
	std::vector<Vector2D> slime_list_position = { Vector2D(500.0f, 320.0f), Vector2D(700.0f, 360.0f), Vector2D(900.0f, 360.0f)
												, Vector2D(900.0f, 360.0f), Vector2D(2700.0f, 340.0f), Vector2D(4000.0f, 340.0f)
												, Vector2D(4100.0f, 340.0f) };
	for (auto& slime_position : slime_list_position)
	{
		enemy_list.push_back(CreateObject<Slime>(slime_position));
	}
	////// BigSlime
	enemy_list.push_back(CreateObject<BigSlime>(Vector2D(300.0f, 320.0f)));
	// Ground
	ground = CreateObject<Ground>(Vector2D());
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

Direction IngameScene::VectorPlayertoEnemy(EnemyBase* enemy_base)
{
	Vector2D vec = enemy_base->GetPosition() - player->GetPosition();
	if (vec.x > 0.0f) {
		return Direction::FRONT;
	}
	else
	{
		return Direction::BACK;
	}
}

void IngameScene::PlayertoEnemyAttackEvent(EnemyBase* enemy_base)
{
	if (enemy_base != nullptr)
	{
		player->ApplyDamage(player, enemy_base);
	}
}

void IngameScene::EnemytoPlayerAttackEvent(EnemyBase* enemy_base)
{
	if (enemy_base != nullptr)
	{
		enemy_base->ApplyDamage(enemy_base, player);
	}
}

SceneType IngameScene::OnPlayerDead()
{
	// �v���C���[�c�@��1���炷
	player_life--;
	// �v���C���[�c�@��0�̏ꍇ
	if (player_life == 0)
	{
		// ���ׂẴI�u�W�F�N�g���폜
		DestroyAllObjects();
		// �V�[����GAMEOVER_SCENE�ɕύX���ĕԂ�
		return SceneType::GAMEOVER_SCENE;
	}
	// �v���C���[�c�@��1�ȏ�̏ꍇ
	else
	{
		// �X�e�[�W�̏�����
		DestroyAllObjects();
		InitStage();
		// �V�[���͕ύX���Ȃ�
		return GetSceneType();
	}
}

void IngameScene::OnPlayerGoalReached()
{
	is_goal = true;
}
