#pragma once

#include <string>
#include <tchar.h>

struct GraphicStructure {
public:
	const TCHAR* filename; // �p�X
	int all_num;          // ������
	int num_x;            // ���̕�����
	int num_y;            // �c�̕�����
	int size_x;           // �c��
	int size_y;           // ����
	int frame_num;        // ���t���[���ŉ摜��ύX���邩
};