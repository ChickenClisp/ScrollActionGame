#pragma once

#include <string>
#include <tchar.h>

struct GraphicStructure {
public:
	const TCHAR* filename; // パス
	int all_num;          // 総枚数
	int num_x;            // 横の分割数
	int num_y;            // 縦の分割数
	int size_x;           // 縦幅
	int size_y;           // 横幅
	int frame_num;        // 何フレームで画像を変更するか
};