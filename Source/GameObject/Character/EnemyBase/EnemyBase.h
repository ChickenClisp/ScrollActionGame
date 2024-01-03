#pragma once

#include "../Character.h"

enum class EnemyState : unsigned short
{
	IDLE,
	RUN,
	ATTACK,
	DAMAGE,
	DEAD,
};

class EnemyBase : public Character
{
public:
	EnemyBase();
	virtual ~EnemyBase();

public:
	//~ Begin GameObject interface
	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	virtual void Draw(const Vector2D& screen_offset) override;
	virtual void Finalize() override;
	//~ End GameObject interface
	virtual void OnHitGroundCollision(float hit_mapchip_position, HitCollisionDirection hit_collsion_direction) override;
	float GetSearchRadius() { return search_radius;}
protected:
	void ChangeEnemyState(EnemyState new_state);
	virtual void OnEnterEnemyState(EnemyState state);
	virtual void OnLeaveEnemyState(EnemyState state);
	void UpdateCollisionParams();

protected:
	EnemyState current_enemy_state;
	float search_radius;
};