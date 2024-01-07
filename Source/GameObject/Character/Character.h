#pragma once

#include "../GameObject.h"
#include <map>
#include <vector>

// アニメーション用のenum
enum class AnimType
{
	NO_ANIMATION,
	IDLE,
	WALK,
	BLINK,
	RUN,
	JUMP,
	TO_FALL,
	FALL,
	ATTACK,
	DAMAGED,
	DEAD,
};

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
	int GetAttackPower() { return attack_power; }

protected:
	virtual void OnDamaged(int damage);
	virtual void OnDead();
	// animationの画像ハンドル, speedをセットする
	virtual void SetAnimation(AnimType new_animtype, int new_animation_speed);
	virtual void SetFrameZero();

private:
	void UpdateAnimation();
	

protected:
	int graphic_handle;
	std::map<AnimType, std::vector<int>> graphic_handles_map;
	AnimType animtype;
	int hp;
	int attack_power;
	int animation_frame;
	int animation_frame_adjust;
	int animation_speed;
	Direction current_direction;
	IsGround current_isground;
	Vector2D center_dir;

	Vector2D verocity;
};