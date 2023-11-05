#pragma once

#include "../GameObject.h"
#include <map>
#include <vector>

// �A�j���[�V�����p��enum
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
	void ApplyDamage(int damage);

protected:
	virtual void OnDamaged(int damage);
	// animation�̉摜�n���h��, speed���Z�b�g����
	void SetAnimation(AnimType new_animtype, int new_animation_speed);
	void SetFrameZero();

private:
	void UpdateAnimation();
	

protected:
	int graphic_handle;
	std::map<AnimType, std::vector<int>> graphic_handles_map;
	AnimType animtype;
	int hp;
	int animation_frame;
	int animation_frame_adjust;
	int animation_speed;
};