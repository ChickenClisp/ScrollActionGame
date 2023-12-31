#include "GraphicResourceManager.h"
#include "DxLib.h"

GraphicResourceManager::GraphicResourceManager()
{
}

GraphicResourceManager::~GraphicResourceManager()
{
}

int GraphicResourceManager::LoadGraphicResource(const std::string file_name)
{
	// すでに存在している場合, そのvalueを返す
	if (loaded_graphic_resources.find(file_name) != loaded_graphic_resources.end())
	{
		return loaded_graphic_resources[file_name];
	}
	// 画像を登録する
	loaded_graphic_resources.emplace(file_name, LoadGraph(file_name.c_str()));
	return loaded_graphic_resources[file_name];
}

void GraphicResourceManager::LoadDivGraphicResource(const std::string file_name, int all_num, int x_num, int y_num, int x_size, int y_size, std::vector<int>& handle_buf)
{
	// すでに存在している場合, 何もしない
	if (loaded_graphic_resources.find(file_name) != loaded_graphic_resources.end())
	{
		return;
	}
	// 画像を登録する
	handle_buf.resize(all_num);
	LoadDivGraph(file_name.c_str(), all_num, x_num, y_num, x_size, y_size, handle_buf.data());
	loaded_graphic_resources.emplace(file_name, handle_buf[0]);
	
	return ;
}

void GraphicResourceManager::UnloadGraphicResource(int graphic_handle)
{
	for (auto iterator = loaded_graphic_resources.begin(); iterator != loaded_graphic_resources.end(); iterator++)
	{
		if (iterator->second == graphic_handle)
		{
			DeleteGraph(graphic_handle);
			loaded_graphic_resources.erase(iterator);
			return;
		}
	}
}

void GraphicResourceManager::UnloadAllGraphicResources()
{
	for (auto& graphic_handle : loaded_graphic_resources)
	{
		DeleteGraph(graphic_handle.second);
	}
	loaded_graphic_resources.clear();
}
