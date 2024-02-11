#pragma once

#include "../Character.h"
#include "../../../GameObject/Sword/Sword.h"
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
	const float INVINCIBLE_TIMER = 120.0f; // 無敵時間
	const float NOCKBACK_DELTA_POSITION = 20.0f; // ノックバックで動く距離
	const float MAX_SPEED = 200.0f;  // 最大スピード(x方向)
	const float JUMP_POWER = 500.0f; // ジャンプ力(パルス)
	const float GRAVITY = 30.0f;     // 重力加速度

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
	virtual void OnHitObject(class GameObject* opponent_gameobject)override;
	bool GetInvincibleMode() { return is_invincible; }
	void SetInvincibleMode(bool b_invincible, float timer) { is_invincible = b_invincible; invincible_timer = timer;}
	/**
	 * シーンのセット
	 * @param	new_sword	セットするsword
	 */
	void SetSword(class Sword* new_sword){ equipped_sword = new_sword; }

protected:
	virtual void OnDamaged(Character* attack_character, Character* damaged_character) override;
	virtual void OnDead() override;

private:
	void UpdateInvincibleTimer();
	void UpdateInput();
	void UpdateCheckConditionChangePlayerState(PlayerState state);
	void UpdateRun();
	void UpdateJump();
	void UpdateAttack();
	void ChangePlayerState(PlayerState new_state);
	void OnEnterPlayerState(PlayerState state);
	void OnLeavePlayerState(PlayerState state);
	void OnGoalReached();

public:
	bool is_active;
private:
	PlayerState current_player_state;
	class Sword* equipped_sword; 
	bool is_invincible;               // 無敵モード
	float invincible_timer;           // 無敵時間
	int key[256];
};