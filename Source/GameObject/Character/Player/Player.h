#pragma once

#include "../Character.h"
#include "../Source/Utility/GraphicStructure.h"

#define PLAYER_IDLE   (GraphicStructure{"Resources/Images/collon_wait_a.bmp", 4, 4, 1, 128, 128, 10})
#define PLAYER_RUN    (GraphicStructure{"Resources/Images/collon_run8.bmp", 8, 4, 2, 128, 128, 5})
#define PLAYER_JUMP   (GraphicStructure{"Resources/Images/collon_jump.bmp", 6, 4, 2, 128, 128, 10})
#define PLAYER_ATTACK (GraphicStructure{"Resources/Images/collon_attack.bmp", 3, 3, 1, 128, 128, 15})
#define PLAYER_DAMAGE (GraphicStructure{"Resources/Images/collon_damage.bmp", 1, 1, 1, 128, 128, 10})


enum class PlayerState : unsigned short
{
	IDLE,
	RUN,
	JUMP,
	ATTACK,
	DAMAGE,
	DEAD,
};

class Player : public Character
{
public:
	Player();
	virtual ~Player();

public:
	//~ Begin GameObject interface
	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	virtual void Draw(const Vector2D& screen_offset) override;
	virtual void Finalize() override;
	//~ End GameObject interface
	virtual void OnHitGroundCollision(float hit_mapchip_position, HitCollisionDirection hit_collsion_direction) override;
private:
	void ChangePlayerState(PlayerState new_state);
	void OnEnterPlayerState(PlayerState state);
	void OnLeavePlayerState(PlayerState state);
	void UpdateInput();
	void UpdateCollisionParams();
private:
	PlayerState current_player_state;
	int key[256];
};