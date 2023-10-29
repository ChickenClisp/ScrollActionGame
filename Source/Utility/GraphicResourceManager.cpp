#include "GraphicResourceManager.h"

GraphicResourceManager::GraphicResourceManager()
{
}

GraphicResourceManager::~GraphicResourceManager()
{
}

GraphicResourceManager GraphicResourceManager::GetInstance()
{
	return GraphicResourceManager();
}

int GraphicResourceManager::LoadGraphicResource(const TCHAR* file_name)
{
	return 0;
}

void GraphicResourceManager::UnloadGraphicResource(int graphic_handle)
{
}

void GraphicResourceManager::UnloadAllGraphicResources()
{
}
