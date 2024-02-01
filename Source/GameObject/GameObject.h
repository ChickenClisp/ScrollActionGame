#pragma once

#include "../Utility/Vector2D.h"
#include "../Utility/CollisionParams.h"
#include "../Utility/HitCollisionDirection.h"


/**
 * �Q�[�����ɕ\�������I�u�W�F�N�g�̊��N���X
 */
class GameObject
{
public:
	GameObject();
	virtual ~GameObject() {}

public:
	virtual void Initialize() {}
	virtual void Update(float delta_seconds) {}
	virtual void Draw(const Vector2D& screen_offset) {}
	virtual void Finalize() {}

	/**
	 * �V�[���̎擾
	 * @return owner_scene
	 */
	class SceneBase* GetOwnerScene() const { return owner_scene; }

	/**
	 * �V�[���̃Z�b�g
	 * @param	new_owner_scene	�Z�b�g����V�[��
	 */
	void SetOwnerScene(class SceneBase* new_owner_scene);

	/**
	 * Position�̎擾
	 * @return	position
	 */
	Vector2D GetPosition() const { return position; }

	/**
	 * Position�̃Z�b�g
	 * @param	new_position	�Z�b�g����Position
	 */
	void SetPosition(const Vector2D& new_position);

	/**
	 * �`�揇�̎擾
	 * @return	draw_sort_priority
	 */
	int GetDrawSortPriority() const { return draw_sort_priority; }

	/**
	 * �`�揇�̃Z�b�g
	 * @param	new_priority	�Z�b�g����`�揇
	 */
	void SetDrawSortPriority(int new_priority);

	/**
	 * �R���W�����p�����[�^�̃Z���^�[�|�W�V�����̍X�V
	 * �iSetPosition()�������ƂɎg���j
	 */
	void UpdateCollisionParamsCenterPosition(class GameObject* gameobject);

	/**
	 * �}�b�v�`�b�v�Ƃ̏Փ˃C�x���g(�����߂�)
	 * @param hit_mapchip_position   �Փ˂����}�b�v�`�b�v�̈ʒu
	 *		  hit_collsion_direction �Փ˂�����
	 */
	virtual void OnHitGroundCollision(float hit_mapchip_position, HitCollisionDirection hit_collsion_direction) {}

	/**
	 * ���I�u�W�F�N�g�Ƃ̏Փ˃C�x���g
	 * @param opponent_gameobject �Փ˂������I�u�W�F�N�g
	 */
	virtual void OnHitObject(class GameObject* opponent_gameobject) {}

	Vector2D GetPrevPosition() const { return prev_position; }
	Vector2D GetDeltaPosition() const { return delta_position; }

	CollisionParams GetCollisionParams() const { return body_collision_params; }
	void SetCollisionParams(CollisionParams& collision_params);

protected:
	// �I�[�i�[�ƂȂ�V�[��
	class SceneBase* owner_scene;

	// �ʒu
	Vector2D position;
	// 1�t���[���O�̈ʒu
	Vector2D prev_position;
	// �t���[���ԍ���
	Vector2D delta_position;

	// �`�揇�B���l��������������`����s��
	int draw_sort_priority;

	// �R���W�����p�����[�^
	CollisionParams  body_collision_params;

	// position����body_collision_params.center_position�܂ł̃x�N�g��
	Vector2D center_dir;
};