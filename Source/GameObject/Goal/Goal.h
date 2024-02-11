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
};