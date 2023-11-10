#include "Ground.h"
#include "../Source/Utility/GraphicResourceManager.h"
#include <tchar.h>
#include <DxLib.h>

Ground::Ground()
	:ground_data()
{
}

Ground::~Ground()
{
}

void Ground::Initialize()
{
	__super::Initialize();
	// âÊëúÇÃì«Ç›çûÇ›
	GraphicResourceManager& graphic_resource_manager = GraphicResourceManager::GetInstance();
	int graphic_handle;
	graphic_handle = graphic_resource_manager.LoadGraphicResource(_T("Resources/Images/mapchips/mapchip_037.bmp"));
	mapchips_data.push_back(graphic_handle);
	graphic_handle = graphic_resource_manager.LoadGraphicResource(_T("Resources/Images/mapchips/mapchip_038.bmp"));
	mapchips_data.push_back(graphic_handle);
	graphic_handle = graphic_resource_manager.LoadGraphicResource(_T("Resources/Images/mapchips/mapchip_039.bmp"));
	mapchips_data.push_back(graphic_handle);
	graphic_handle = graphic_resource_manager.LoadGraphicResource(_T("Resources/Images/mapchips/mapchip_040.bmp"));
	mapchips_data.push_back(graphic_handle);
	graphic_handle = graphic_resource_manager.LoadGraphicResource(_T("Resources/Images/mapchips/mapchip_041.bmp"));
	mapchips_data.push_back(graphic_handle);
	graphic_handle = graphic_resource_manager.LoadGraphicResource(_T("Resources/Images/mapchips/mapchip_042.bmp"));
	mapchips_data.push_back(graphic_handle);
	graphic_handle = graphic_resource_manager.LoadGraphicResource(_T("Resources/Images/mapchips/mapchip_043.bmp"));
	mapchips_data.push_back(graphic_handle);
	graphic_handle = graphic_resource_manager.LoadGraphicResource(_T("Resources/Images/mapchips/mapchip_044.bmp"));
	mapchips_data.push_back(graphic_handle);
	graphic_handle = graphic_resource_manager.LoadGraphicResource(_T("Resources/Images/mapchips/mapchip_045.bmp"));
	mapchips_data.push_back(graphic_handle);
	graphic_handle = graphic_resource_manager.LoadGraphicResource(_T("Resources/Images/mapchips/mapchip_000.bmp"));
	mapchips_data.push_back(graphic_handle);
}

void Ground::Update(float delta_seconds)
{
}

void Ground::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);

	// ground_dataÇÃèÓïÒÇÇ‡Ç∆Ç…ï`âÊ
	for (int i = 0; i < NUM_MAP_HEIGHT; i++)
	{
		// yç¿ïWÇÃê›íË
		position.y = (float)SIZE_MAP_HEIGHT * i;
		for (int j = 0; j < NUM_MAP_WIDTH; j++)
		{
			int chip_id = ground_data[i][j];
			// xç¿ïWÇÃê›íË
			position.x = (float)SIZE_MAP_WIDTH * j;
			// ï`âÊÇ∑ÇÈ
			DrawGraph(position.x, position.y, mapchips_data[chip_id], true);
		}
	}
}

void Ground::Finalize()
{
	__super::Finalize();

	// âÊëúÇÃîjä¸
	for (auto& handle : mapchips_data) {
		DeleteGraph(handle);
	}
	/*
	for (int i = 0; i < NUM_MAP_HEIGHT; i++)
	{
		for (int j = 0; j < NUM_MAP_WIDTH; j++)
		{
			DeleteGraph(mapchips_data[i + j * NUM_MAP_WIDTH]);
			mapchips_data[i + j * NUM_MAP_WIDTH] = 0;
		}
	}
	*/
}
