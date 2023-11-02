#include "BackImage.h"
#include "../../SystemTypes.h"
#include "../../Utility/GraphicResourceManager.h"
#include <DxLib.h>

BackImage::BackImage()
	: graphic_handle_scroll(0)
{
}

BackImage::~BackImage()
{
}

void BackImage::Initialize()
{
	// �摜�̓ǂݍ���
	GraphicResourceManager& graphic_resource_manager = GraphicResourceManager::GetInstance();
	graphic_handle_scroll = graphic_resource_manager.LoadGraphicResource(_T("Resources/Images/backimage.png"));
	//graphic_handle_scroll = LoadGraph(_T("Resources/Images/backimage.png"));
}

void BackImage::Update(float delta_seconds)
{
	
}

void BackImage::Draw(const Vector2D& screen_offset)
{
	// �e�N���X��Draw()
	__super::Draw(screen_offset);

	// �X�N���[�����W�ɕϊ����Ĕw�i�摜�̕`��(�X�N���[��)
	int screen_offset_x, screen_offset_y;
	screen_offset.ToInt(screen_offset_x, screen_offset_y);

	int position_integer_per_screensize = screen_offset_x / SCREEN_RESOLUTION_X; // �X�e�[�W�T�C�Y���X�N���[���T�C�Y���ƂɊ������z��̗v�f�ԍ�
	if (position_integer_per_screensize % 2 == 0)
	{
		DrawGraph(SCREEN_RESOLUTION_X * position_integer_per_screensize - screen_offset_x, 0 - screen_offset_y, graphic_handle_scroll, true);
		DrawTurnGraph(SCREEN_RESOLUTION_X * (position_integer_per_screensize + 1) - screen_offset_x, 0 - screen_offset_y, graphic_handle_scroll, true);
	}
	else
	{
		DrawTurnGraph(SCREEN_RESOLUTION_X * position_integer_per_screensize - screen_offset_x, 0 - screen_offset_y, graphic_handle_scroll, true);
		DrawGraph(SCREEN_RESOLUTION_X * (position_integer_per_screensize + 1) - screen_offset_x, 0 - screen_offset_y, graphic_handle_scroll, true);
	}
}

void BackImage::Finalize()
{
}
