#pragma once
#include <map>
#include <Windows.h>

class GraphicResourceManager
{
public:
	GraphicResourceManager();
	virtual ~GraphicResourceManager();

public:
	GraphicResourceManager GetInstance();
	int LoadGraphicResource(const TCHAR* file_name);
	void UnloadGraphicResource(int graphic_handle);
	void UnloadAllGraphicResources();

private:
	std::map<TCHAR*, int> loaded_graphic_resources;
};