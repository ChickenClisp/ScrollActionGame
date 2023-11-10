#pragma once
#include "../GameObject.h"

#include <vector>

/*
	ステージ：プレイヤーが動き回るステージ全体
	マップ：描画されている部分
	マップチップ：配置するマップチップ画像全体
*/
#define STAGE_WIDTH 3000                              // ステージの横幅
#define STAGE_HEIGHT 480                               // ステージの縦幅

#define SIZE_MAP_WIDTH 32                              // マップチップの横幅
#define SIZE_MAP_HEIGHT 32                             // マップチップの縦幅

#define NUM_MAP_WIDTH 24
#define NUM_MAP_HEIGHT 15
//#define NUM_MAP_WIDTH (STAGE_WIDTH / SIZE_MAP_WIDTH)   // マップの横チップ数
//#define NUM_MAP_HEIGHT (STAGE_HEIGHT /SIZE_MAP_HEIGHT) // マップの縦チップ数

#define SIZE_CHIP_WIDTH 32                             // マップチップの横幅
#define SIZE_CHIP_HEIGHT 32							   // マップチップの縦幅
#define NUM_CHIP_WIDTH 8							   // マップチップの横チップ数
#define NUM_CHIP_HEIGHT 8							   // マップチップの縦チップ数



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
};