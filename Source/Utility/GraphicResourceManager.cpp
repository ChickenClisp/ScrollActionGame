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
	// ‚·‚Å‚É‘¶İ‚µ‚Ä‚¢‚éê‡, ‚»‚Ìvalue‚ğ•Ô‚·
	if (loaded_graphic_resources.find(file_name) != loaded_graphic_resources.end())
	{
		return loaded_graphic_resources[file_name];
	}
	// ‰æ‘œ‚ğ“o˜^‚·‚é
	loaded_graphic_resources.emplace(file_name, LoadGraph(file_name.c_str()));
	return loaded_graphic_resources[file_name];
}

void GraphicResourceManager::LoadDivGraphicResource(const std::string file_name, int all_num, int x_num, int y_num, int x_size, int y_size, std::vector<int>& handle_buf)
{
	// ‚·‚Å‚É‘¶İ‚µ‚Ä‚¢‚éê‡, ‚»‚Ìvalue‚ğ•Ô‚·
	if (loaded_graphic_resources.find(file_name) != loaded_graphic_resources.end())
	{
		handle_buf.push_back(loaded_graphic_resources[file_name]);
		for (int i = 1; i < all_num; i++)
		{
			handle_buf.push_back(loaded_graphic_resources[file_name + std::to_string(i)]);
		}
		return;
	}
	// ‰æ‘œ‚ğ“o˜^‚·‚é
	handle_buf.resize(all_num);
	LoadDivGraph(file_name.c_str(), all_num, x_num, y_num, x_size, y_size, handle_buf.data());

	loaded_graphic_resources.emplace(file_name, handle_buf[0]);
	for (int i = 1; i < all_num; i++)
	{
		loaded_graphic_resources.emplace(file_name + std::to_string(i), handle_buf[i]);
	}
	
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
