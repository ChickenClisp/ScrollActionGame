#pragma once

#include "../GameObject.h"

enum class Direction : unsigned short
{
	FRONT,
	BACK,
};

enum class IsGround : unsigned short
{
	OnGround,
	InAir,
};

class Character : public GameObject
{
public:
	Character();
	virtual ~Character();

public:
	//~ Begin GameObject interface
	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	virtual void Draw(const Vector2D& screen_offset) override;
	virtual void Finalize() override;
	//~ End GameObject interface
	void ApplyDamage(int damage, Character* damaged_character);
	int GetAttackPower() const { return attack_power; }

protected:
	virtual void OnDamaged(int damage, class Character* damaged_character);
	virtual void OnDead();

protected:
	int hp;
	int attack_power;
	Direction current_direction;
	IsGround current_isground;
	Vector2D verocity;
};