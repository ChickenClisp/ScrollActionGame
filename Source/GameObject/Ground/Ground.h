#pragma once
#include "../GameObject.h"

#include <vector>

/*
	�X�e�[�W�F�v���C���[���������X�e�[�W�S��
	�}�b�v�F�`�悳��Ă��镔��
	�}�b�v�`�b�v�F�z�u����}�b�v�`�b�v�摜�S��
*/
#define STAGE_WIDTH 3000                              // �X�e�[�W�̉���
#define STAGE_HEIGHT 480                               // �X�e�[�W�̏c��

#define SIZE_MAP_WIDTH 32                              // �}�b�v�`�b�v�̉���
#define SIZE_MAP_HEIGHT 32                             // �}�b�v�`�b�v�̏c��

#define NUM_MAP_WIDTH 24
#define NUM_MAP_HEIGHT 15
//#define NUM_MAP_WIDTH (STAGE_WIDTH / SIZE_MAP_WIDTH)   // �}�b�v�̉��`�b�v��
//#define NUM_MAP_HEIGHT (STAGE_HEIGHT /SIZE_MAP_HEIGHT) // �}�b�v�̏c�`�b�v��

#define SIZE_CHIP_WIDTH 32                             // �}�b�v�`�b�v�̉���
#define SIZE_CHIP_HEIGHT 32							   // �}�b�v�`�b�v�̏c��
#define NUM_CHIP_WIDTH 8							   // �}�b�v�`�b�v�̉��`�b�v��
#define NUM_CHIP_HEIGHT 8							   // �}�b�v�`�b�v�̏c�`�b�v��



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