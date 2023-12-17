#include "Ground.h"
#include "../../SystemTypes.h"
#include "../Source/Utility/GraphicResourceManager.h"
#include <tchar.h>
#include <DxLib.h>
#include <cmath>

Ground::Ground()
	:ground_data()
	,mapchips_data()
	,num_screen_width(0)
	,num_screen_height(0)
{
}

Ground::~Ground()
{
}

void Ground::Initialize()
{
	__super::Initialize();

	// �X�N���[���̉��`�b�v���Əc�`�b�v�������߂�
	num_screen_width = SCREEN_RESOLUTION_X / SIZE_CHIP_WIDTH;
	num_screen_height = SCREEN_RESOLUTION_Y / SIZE_CHIP_HEIGHT;

	// �摜�̓ǂݍ���
	GraphicResourceManager& graphic_resource_manager = GraphicResourceManager::GetInstance();
	int graphic_handle;
	mapchips_data.push_back(0); // csv�t�@�C����0��null��\������

	std::vector<int> out_sprite_handles;
	graphic_resource_manager.LoadDivGraphicResource("Resources/Images/oak_woods_tileset.png", 21 * 15, 21, 15, 24, 24, out_sprite_handles);
	// ###�ȉ��͂Ƃ肠�����ŏ������̂ŁA���ƂŕҏW����###
	mapchips_data.push_back(out_sprite_handles[22]);
	mapchips_data.push_back(out_sprite_handles[21]);
	mapchips_data.push_back(out_sprite_handles[42]);
	mapchips_data.push_back(out_sprite_handles[24]);
	mapchips_data.push_back(out_sprite_handles[45]);
	mapchips_data.push_back(out_sprite_handles[4]);
	mapchips_data.push_back(out_sprite_handles[4]);
	mapchips_data.push_back(out_sprite_handles[4]);
	mapchips_data.push_back(out_sprite_handles[4]);
	mapchips_data.push_back(out_sprite_handles[1]); //10
	mapchips_data.push_back(out_sprite_handles[2]);
	mapchips_data.push_back(out_sprite_handles[0]);
	mapchips_data.push_back(out_sprite_handles[3]);
	mapchips_data.push_back(out_sprite_handles[279]);
	mapchips_data.push_back(out_sprite_handles[278]);
	mapchips_data.push_back(out_sprite_handles[4]);
	mapchips_data.push_back(out_sprite_handles[4]);
	mapchips_data.push_back(out_sprite_handles[4]);
	mapchips_data.push_back(out_sprite_handles[4]);
	mapchips_data.push_back(out_sprite_handles[6]); //20
	mapchips_data.push_back(out_sprite_handles[7]);
	mapchips_data.push_back(out_sprite_handles[5]);
	mapchips_data.push_back(out_sprite_handles[8]);
	mapchips_data.push_back(out_sprite_handles[276]);
	mapchips_data.push_back(out_sprite_handles[275]);
	mapchips_data.push_back(out_sprite_handles[273]);
	mapchips_data.push_back(out_sprite_handles[231]);
	mapchips_data.push_back(out_sprite_handles[4]);
	mapchips_data.push_back(out_sprite_handles[4]);
	mapchips_data.push_back(out_sprite_handles[180]); //30
	mapchips_data.push_back(out_sprite_handles[181]);
	mapchips_data.push_back(out_sprite_handles[68]);
	mapchips_data.push_back(out_sprite_handles[70]);
	mapchips_data.push_back(out_sprite_handles[48]);
	mapchips_data.push_back(out_sprite_handles[4]);
	mapchips_data.push_back(out_sprite_handles[4]);
	mapchips_data.push_back(out_sprite_handles[4]);
	mapchips_data.push_back(out_sprite_handles[4]);
	mapchips_data.push_back(out_sprite_handles[10]);
	mapchips_data.push_back(out_sprite_handles[222]); //40
	mapchips_data.push_back(out_sprite_handles[223]);
	mapchips_data.push_back(out_sprite_handles[72]);
	mapchips_data.push_back(out_sprite_handles[74]);
	mapchips_data.push_back(out_sprite_handles[52]);
	mapchips_data.push_back(out_sprite_handles[64]);
	mapchips_data.push_back(out_sprite_handles[111]);
	mapchips_data.push_back(out_sprite_handles[237]);
	mapchips_data.push_back(out_sprite_handles[236]); 
	mapchips_data.push_back(out_sprite_handles[4]);
	mapchips_data.push_back(out_sprite_handles[4]);//50

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

	// �`����J�n����ꏊ�i�`�b�v�z����W�j�����߂�
	int draw_start_position_x = std::floor(screen_offset_x / SIZE_CHIP_WIDTH);
	int draw_start_position_y = std::floor(screen_offset_y / SIZE_CHIP_HEIGHT);

	
	// ground_data�̏������Ƃɕ`��
	for (int y = 0; y < num_screen_height; y++)
	{
		// y���W�̐ݒ�
		int chip_position_y = y + draw_start_position_y;
		position.y = (float)SIZE_CHIP_HEIGHT * chip_position_y;
		for (int x = 0; x < num_screen_width + 1; x++) //1�`�b�v�������`�悷�邱�ƂłȂ߂炩�ɕ\���ł���
		{
			// x���W�̐ݒ�
			int chip_position_x = x + draw_start_position_x;
			position.x = (float)SIZE_CHIP_WIDTH * chip_position_x;
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
	for (auto& handle: mapchips_data) {
		DeleteGraph(handle);
	}
}
