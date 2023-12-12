#pragma once
#include "../GameObject.h"
#include "../Source/Scene/SceneBase.h"

#include <vector>

/*
	�X�e�[�W�F�v���C���[���������X�e�[�W�S��
	�}�b�v�F�`�悳��Ă��镔��
	�}�b�v�`�b�v�F�z�u����}�b�v�`�b�v�摜�S��
*/
#define SIZE_CHIP_WIDTH 24                             // �}�b�v�`�b�v�̉���
#define SIZE_CHIP_HEIGHT 24							   // �}�b�v�`�b�v�̏c��


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
	int num_screen_width;  // �X�N���[���̉��`�b�v��
	int num_screen_height; // �X�N���[���̏c�`�b�v��
};