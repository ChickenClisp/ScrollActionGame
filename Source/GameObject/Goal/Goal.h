#pragma once
#include "../GameObject.h"

class Goal : public GameObject
{
public:
	Goal();
	virtual ~Goal();

public:
	//~ Begin GameObject interface
	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	virtual void Draw(const Vector2D& screen_offset) override;
	virtual void Finalize() override;
	//~ End GameObject interface

public:
	//bool GetActive() const { return is_active; }
	//void SetActive(bool b_active) { is_active = b_active; }

private:
	//bool is_active;
};