#pragma once

class InputManager
{
private:
	InputManager();
	virtual ~InputManager();

public:
	// Singleton
	InputManager(const InputManager&) = delete;
	InputManager& operator = (const InputManager&) = delete;
	InputManager(InputManager&&) = delete;
	InputManager& operator=(InputManager&&) = delete;
	static InputManager& GetInstance()
	{
		static InputManager instance;
		return instance;
	}
	
	// 押しているかどうかを判断
	bool GetKey(int key_code);
	// 押された瞬間を判断
	bool GetKeyDown(int key_code);

private:
	int key[256];

};