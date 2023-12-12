#pragma once
#include "../GameObject.h"
#include "../Source/Scene/SceneBase.h"

#include <vector>

/*
	ステージ：プレイヤーが動き回るステージ全体
	マップ：描画されている部分
	マップチップ：配置するマップチップ画像全体
*/
#define SIZE_CHIP_WIDTH 24                             // マップチップの横幅
#define SIZE_CHIP_HEIGHT 24							   // マップチップの縦幅


class Ground : public GameObject
{
public:
	Ground();
	virtual ~Ground();

public:
	//~ Begin GameObject interface
	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	virtual void Draw(const Vector2D& screen_offset) override;
	virtual void Finalize() override;
	//~ End GameObject interface

	void SetGroundData(const std::vector<std::vector<int>>& new_ground_data)
	{
		ground_data = new_ground_data;
	}

	std::vector<std::vector<int>> GetGroundData()
	{
		return ground_data;
	}

private:
	std::vector<std::vector<int>> ground_data;
	std::vector<int> mapchips_data;
	int num_screen_width;  // スクリーンの横チップ数
	int num_screen_height; // スクリーンの縦チップ数
};