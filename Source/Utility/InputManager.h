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
	
	// �����Ă��邩�ǂ����𔻒f
	bool GetKey(int key_code);
	// �����ꂽ�u�Ԃ𔻒f
	bool GetKeyDown(int key_code);

private:
	int key[256];

};