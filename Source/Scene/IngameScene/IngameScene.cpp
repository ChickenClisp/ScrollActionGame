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
	// 親クラスのInitialize()
	__super::Initialize();

	stage_size = 3000;

	// Objectを生成
	CreateObject<BackImage>(Vector2D(SCREEN_RESOLUTION_X / 2.0f, SCREEN_RESOLUTION_Y / 2.0f));
	player = CreateObject<Player>(Vector2D(SCREEN_RESOLUTION_X / 2.0f, SCREEN_RESOLUTION_Y / 2.0f));
	ground = CreateObject<Ground>(Vector2D(0, 0));
	
	// マップの読み込み
	std::vector<std::vector<int>> stage_data = ground->GetGroundData();
	LoadCSV("Resources/stage1.csv", stage_data);
	ground->SetGroundData(stage_data);
}

SceneType IngameScene::Update(float delta_seconds)
{
	// 参考URL https://yttm-work.jp/gmpg/2d_game/2d_game_0002.html
	// 参考URL https://yttm-work.jp/gmpg/2d_game/2d_game_0005.html

	// カメラ座標の更新
	camera_position = player->GetPosition();
	// x軸のステージの内外判定
	if (camera_position.x - float(SCREEN_RESOLUTION_X / 2) <= 0.0f)
	{
		camera_position.x = float(SCREEN_RESOLUTION_X / 2);
	}
	if (camera_position.x + float(SCREEN_RESOLUTION_X / 2) >= float(stage_size))
	{
		camera_position.x = float(stage_size) - float(SCREEN_RESOLUTION_X / 2);
	}

	// スクロール用変数(x座標)の更新
	screen_offset.x = camera_position.x - float(SCREEN_RESOLUTION_X / 2);

	// 親クラスのUpdate()
	return __super::Update(delta_seconds);

}

void IngameScene::Draw()
{
	// 親クラスのDraw()
	__super::Draw();
}

void IngameScene::Finalize()
{
	// 親クラスのFinalize()
	__super::Finalize();
}

void IngameScene::LoadCSV(const std::string& filename, std::vector<std::vector<int>>& stage_data)
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
		stage_data.emplace_back();
		for (int j = 0; j < strvec.size(); j++)
		{
			stage_data[i].push_back(std::stoi(strvec.at(j)));
		}
	}
}
