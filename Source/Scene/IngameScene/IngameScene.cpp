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
	// 親クラスのInitialize()
	__super::Initialize();

	// 変数の初期化
	is_goal = false;
	player_life = 3;

	// マップの読み込み
	LoadCSV("Resources/stage2.csv", stage_data);
	stage_size.x = (stage_data[0].size() - 1) * SIZE_CHIP_WIDTH; // -1の理由：右端の列がすべて０のプレイヤー禁止エリアがあるため
	stage_size.y = (stage_data.size() - 2) * SIZE_CHIP_HEIGHT;   // -2の理由：下端2行がすべて０のプレイヤー禁止エリアがあるため

	// Objectを生成
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
	// 親クラスのUpdate()
	SceneType result_scene_type = __super::Update(delta_seconds);

	// 参考URL https://yttm-work.jp/gmpg/2d_game/2d_game_0002.html
	// 参考URL https://yttm-work.jp/gmpg/2d_game/2d_game_0005.html

	// カメラ座標の更新
	camera_position = player->GetPosition();
	// x軸のステージの内外判定
	if (camera_position.x - float(SCREEN_RESOLUTION_X / 2) <= 0.0f)
	{
		camera_position.x = float(SCREEN_RESOLUTION_X / 2);
	}
	if (camera_position.x + float(SCREEN_RESOLUTION_X / 2) >= float(stage_size.x))
	{
		camera_position.x = float(stage_size.x) - float(SCREEN_RESOLUTION_X / 2);
	}

	// スクロール用変数(x座標)の更新
	screen_offset.x = camera_position.x - float(SCREEN_RESOLUTION_X / 2);

	// playerチェック
	if (player->is_active == false)
	{
		OnPlayerDead();
	}
	
	return result_scene_type;
}

void IngameScene::Draw()
{
	// 親クラスのDraw()
	__super::Draw();

	// ゴールした場合、GOAL!!を表示
	if (is_goal == true)
	{
		DrawString(SCREEN_RESOLUTION_X / 2.0f - 30.0f, SCREEN_RESOLUTION_Y / 2.0f, "GOAL!!", GetColor(255, 255, 255));
	}
	// プレイヤーの残機が0になった場合、Game Over...を表示
	if (player_life == 0)
	{
		DrawString(SCREEN_RESOLUTION_X / 2.0f - 100.0f, SCREEN_RESOLUTION_Y / 2.0f, "Game Over...", GetColor(255, 255, 255));
	}
}

void IngameScene::Finalize()
{
	// 親クラスのFinalize()
	__super::Finalize();
}

void IngameScene::LoadCSV(const std::string& filename, std::vector<std::vector<int>>& ground_data)
{
	// ステージ情報をcsvファイルから取得
	std::ifstream ifs(filename);
	std::string line;
	for (int i = 0; getline(ifs, line); i++)
	{
		// ","で文字列を分割する
		std::istringstream stream(line);
		std::string field;
		std::vector<std::string> strvec;
		while (getline(stream, field, ',')) {
			strvec.push_back(field);
		}
		// stage_dataにint型で格納
		ground_data.emplace_back();
		for (int j = 0; j < strvec.size(); j++)
		{
			ground_data[i].push_back(std::stoi(strvec.at(j)));
		}
	}
}

void IngameScene::InitStage()
{
	// Objectを生成
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
	// プレイヤーが無敵モードなら索敵できないとする
	if (player->GetInvincibleMode())
	{
		return false;
	}
	// プレイヤーと敵の距離を計算
	Vector2D distance = Vector2D((player->GetCollisionParams().center_position.x - enemy_base->GetCollisionParams().center_position.x),
		(player->GetCollisionParams().center_position.y - enemy_base->GetCollisionParams().center_position.y));
	// サーチ範囲内ならばtrue、そうでないならばfalseを返す
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
	// プレイヤー残機を1減らす
	player_life--;
	// プレイヤー残機が0の場合
	if (player_life == 0)
	{
		// すべてのオブジェクトを削除
		DestroyAllObjects();
		// シーン遷移
		/*******************
		********************/
	}
	// プレイヤー残機が1以上の場合
	else
	{
		// ステージの初期化
		DestroyAllObjects();
		InitStage();
	}
}

void IngameScene::OnPlayerGoalReached()
{
	is_goal = true;
}
