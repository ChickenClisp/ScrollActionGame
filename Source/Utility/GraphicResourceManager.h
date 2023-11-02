#pragma once
#include <map>
#include <string>
#include <Windows.h>
#include <vector>

class GraphicResourceManager
{
public:
	GraphicResourceManager();
	virtual ~GraphicResourceManager();

public:
	// ƒVƒ“ƒOƒ‹ƒgƒ“
	GraphicResourceManager(const GraphicResourceManager&) = delete;
	GraphicResourceManager& operator = (const GraphicResourceManager&) = delete;
	GraphicResourceManager(GraphicResourceManager&&) = delete;
	GraphicResourceManager& operator=(GraphicResourceManager&&) = delete;
	static GraphicResourceManager& GetInstance()
	{
		static GraphicResourceManager instance;
		return instance;
	}

	int LoadGraphicResource(const std::string file_name);
	void LoadDivGraphicResource(const std::string file_name, int all_num, int x_num, int y_num, int x_size, int y_size, std::vector<int>& hundle_buf);
	void UnloadGraphicResource(int graphic_handle);
	void UnloadAllGraphicResources();

private:
	std::map <std::string, int> loaded_graphic_resources;
};