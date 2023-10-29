#pragma once

#include "../GameObject.h"


class Character : public GameObject
{
public:
	Character();
	virtual ~Character();

public:
	void ApplyDamage(int damage);

protected:
	virtual void OnDamaged(int damage);

protected:
	int graphic_handle;
	int hp;
};