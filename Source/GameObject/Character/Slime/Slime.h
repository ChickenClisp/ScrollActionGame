#pragma once

#include "../Character.h"
#include "../Source/Utility/GraphicStructure.h"

#define SLIME_IDLE   (GraphicStructure{"Resources/Images/Enemy/Slime/slime_idle.png", 4, 4, 1, 32, 32, 10})
#define SLIME_RUN    (GraphicStructure{"Resources/Images/Enemy/Slime/slime_run.png", 4, 4, 1, 32, 32, 5})
// #define SLIME_JUMP   (GraphicStructure{"Resources/Images/Enemy/Slime/slime_jump.png", 6, 4, 2, 128, 128, 10})
#define SLIME_ATTACK (GraphicStructure{"Resources/Images/Enemy/Slime/slime_attack.png", 5, 5, 1, 32, 32, 15})
// #define SLIME_DAMAGE (GraphicStructure{"Resources/Images/Enemy/Slime/slime_hurt.png", 4, 4, 1, 32, 32, 10})
#define SLIME_DEAD (GraphicStructure{"Resources/Images/Enemy/Slime/slime_die.png", 4, 4, 1, 32, 32, 10})

enum class SlimeState : unsigned short
{
	IDLE,
	RUN,
	ATTACK,
	DAMAGE,
	DEAD,
};

enum class SlimeDirection : unsigned short
{
	FRONT,
	BACK,
};

enum class SlimeIsGround : unsigned short
{
	OnGround,
	InAir,
};

class Slime : public Character
{
public:
	Slime();
	virtual ~Slime();

public:
	//~ Begin GameObject interface
	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	virtual void Draw(const Vector2D& screen_offset) override;
	virtual void Finalize() override;
	//~ End GameObject interface
	virtual void OnHitGroundCollision(float hit_mapchip_position, HitCollisionDirection hit_collsion_direction) override;
private:
	void ChangeSlimeState(SlimeState new_state);
	void OnEnterSlimeState(SlimeState state);
	void OnLeaveSlimeState(SlimeState state);
	void UpdateCollisionParams();

private:
	SlimeState current_slime_state;
	SlimeDirection current_slime_direction;
	SlimeIsGround current_slime_isground;
	Vector2D verocity;
	int key[256];
};