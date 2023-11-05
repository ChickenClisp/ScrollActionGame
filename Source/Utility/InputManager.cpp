#include "InputManager.h"
#include <DxLib.h>

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

bool InputManager::GetKey(int key_code)
{
	if (CheckHitKey(key_code) == 1)
	{
		return true;
	}
	return false;
}

bool InputManager::GetKeyDown(int key_code)
{
	char _key[256];
	GetHitKeyStateAll(_key);
	for (int i = 0; i < 256; i++)
	{
		if (_key[i] != 0)
		{
			key[i]++;
		}
		else
		{
			key[i] = 0;
		}
	}
	if (key[key_code] == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}