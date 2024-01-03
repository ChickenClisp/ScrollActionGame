#pragma once

#include "../EnemyBase.h"
#include "../Source/Utility/GraphicStructure.h"

#define SLIME_IDLE   (GraphicStructure{"Resources/Images/Enemy/Slime/slime_idle.png", 4, 4, 1, 32, 32, 10})
#define SLIME_RUN    (GraphicStructure{"Resources/Images/Enemy/Slime/slime_run.png", 4, 4, 1, 32, 32, 5})
#define SLIME_ATTACK (GraphicStructure{"Resources/Images/Enemy/Slime/slime_attack.png", 5, 5, 1, 32, 32, 15})
// #define SLIME_DAMAGE (GraphicStructure{"Resources/Images/Enemy/Slime/slime_hurt.png", 4, 4, 1, 32, 32, 10})
#define SLIME_DEAD (GraphicStructure{"Resources/Images/Enemy/Slime/slime_die.png", 4, 4, 1, 32, 32, 10})

class Slime : public EnemyBase
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
protected:
	virtual void OnEnterEnemyState(EnemyState state)override;
	virtual void OnLeaveEnemyState(EnemyState state)override;
};