#pragma once

#include "../GameObject.h"

/**
 * サンプルオブジェクト
 * サンプル用に簡易的に実装
 */
class SampleObject : public GameObject
{
public:
	SampleObject();
	virtual ~SampleObject();

public:
	//~ Begin GameObject interface
	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	virtual void Draw(const Vector2D& screen_offset) override;
	virtual void Finalize() override;
	//~ End GameObject interface

private:
	int loaded_sprite_handle;
};