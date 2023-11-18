#include "Ground.h"
#include "../Source/Utility/GraphicResourceManager.h"
#include <tchar.h>
#include <DxLib.h>
#include <cmath>

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
	// �摜�̓ǂݍ���
	GraphicResourceManager& graphic_resource_manager = GraphicResourceManager::GetInstance();
	int graphic_handle;
	mapchips_data.push_back(0); // csv�t�@�C����0��null��\������
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
	// �X�N���[�����W�ɕϊ����ĕ`��
	int screen_offset_x, screen_offset_y;
	screen_offset.ToInt(screen_offset_x, screen_offset_y);

	// �`����J�n����`�b�v�����߂�
	int draw_start_position_x = std::floor(screen_offset_x / SIZE_CHIP_WIDTH);
	int draw_start_position_y = std::floor(screen_offset_y / SIZE_CHIP_HEIGHT);

	// ground_data�̏������Ƃɕ`��
	for (int y = 0; y < NUM_MAP_HEIGHT; y++)
	{
		// y���W�̐ݒ�
		int chip_position_y = y + draw_start_position_y;
		position.y = (float)SIZE_MAP_HEIGHT * chip_position_y;
		for (int x = 0; x < NUM_MAP_WIDTH + 1; x++) //1�`�b�v�������`�悷�邱�ƂłȂ߂炩�ɕ\���ł���
		{
			// x���W�̐ݒ�
			int chip_position_x = x + draw_start_position_x;
			position.x = (float)SIZE_MAP_WIDTH * chip_position_x;
			// �`��
			int chip_id = ground_data[chip_position_y][chip_position_x];
			DrawGraph(position.x - screen_offset_x, position.y - screen_offset_y, mapchips_data[chip_id], true);
		}
	}
}

void Ground::Finalize()
{
	__super::Finalize();

	// �摜�̔j��
	for (auto& handle : mapchips_data) {
		DeleteGraph(handle);
	}
}
