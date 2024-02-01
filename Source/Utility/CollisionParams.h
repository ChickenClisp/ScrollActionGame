#pragma once
#include "Vector2D.h"

/**
 * �R���W�����̃I�u�W�F�N�g�^�C�v
 */
enum class CollisionObjectType : unsigned short
{
	GROUND,
	PLAYER,
	ENEMY,
	ITEM,
	SWORD,
};

/**
 * �R���W�����^�C�v
 */
enum class CollisonType : unsigned short
{
	BLOCK,
	OVERLAP,
};

/**
 * ��`�R���W�����N���X
 */
struct CollisionParams
{
public:
	/**
	 * �^�[�Q�b�g�ɏՓ˂��Ă��邩����
	 */
	bool IsHitCheckTarget(CollisionObjectType target_object_type);

public:
	// ���S���W
	Vector2D center_position;

	// ��`�̒���
	Vector2D box_extent;

	// �R���W�����̃I�u�W�F�N�g�^�C�v
	CollisionObjectType collision_object_type;

	// �ǂ�CollisionObjectType�Ɣ��肷�邩���܂Ƃ߂Ă���p�����[�^
	unsigned int hit_object_types;

	// �R���W�����^�C�v
	CollisonType collision_type;
};