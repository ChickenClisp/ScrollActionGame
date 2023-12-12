#pragma once

#include <vector>
#include <string>
#include "../Utility/Vector2D.h"
#include "../GameObject/GameObject.h"
#include "../Utility/HitCollisionDirection.h"

/**
 * �V�[���^�C�v
 */
enum class SceneType : unsigned short
{
	TITLE_SCENE,
	INGAME_SCENE,
	GAMEOVER_SCENE,
};


/**
 * �V�[���̊��N���X
 */
class SceneBase
{
public:
	SceneBase();
	virtual ~SceneBase();

public:
	/**
	 * ������
	 */
	virtual void Initialize();

	/**
	 * �X�V
	 * @param	delta_seconds	�O�t���[���Ƃ̍�������(s)
	 * @return	���ɑJ�ڂ���V�[���^�C�v
	 */
	virtual SceneType Update(float delta_seconds);

	/** 
	 * �`��
	 */
	virtual void Draw();

	/**
	 * ���
	 */
	virtual void Finalize();

	/**
	 * �V�[���^�C�v�̎擾
	 * @return �V�[���^�C�v
	 */
	virtual SceneType GetSceneType() const = 0;

	/**
	 * GameObject�̐���
	 * Template�Ŏw�肵��GameObject�N���X��h�������N���X�̃C���X�^���X��������
	 * @param	Position	�����ʒu
	 * @return	��������GameObject
	 */
	template <class T>
	T* CreateObject(const Vector2D& position)
	{
		// GameObject�̐���
		T* new_instance = new T();
		GameObject* new_object = dynamic_cast<GameObject*>(new_instance);

		// GameObject��h�����Ă��Ȃ��ꍇ�́A�j�����ďI������
		if (new_object == nullptr)
		{
			//�h�����Ă��Ȃ��N���X�̂��߁A�������Ȃ�
			delete new_instance;
			return nullptr;
		}

		// GameObject�̏�����
		new_object->SetOwnerScene(this);
		new_object->SetPosition(position);
		new_object->Initialize();
		objects.push_back(new_object);

		return new_instance;
	}

	/**
	 * GameObject�̔j��
	 * GameObject��j�����A�z�񂩂�폜����
	 * @param	object	�j������I�u�W�F�N�g
	 */
	void DestroyObject(class GameObject* object);

	/**
	 * �S�Ă�GameObject�̔j��
	 * �V�[���ɐ�������Ă���S�ẴI�u�W�F�N�g��j������
	 */
	void DestroyAllObjects();

	/**
	 * �Փ˔���
	 */
	void UpdateCheckCollision();
	/**
	 * �Փ˔���
	 * @param	target �ΏۃI�u�W�F�N�g
	 *			collision_params�@���I�u�W�F�N�g�̃R���W�����p�����[�^
	 *			hit_collision_params �ΏۃI�u�W�F�N�g�̃R���W�����p�����[�^
	 * @return	����̐^�U
	 */
	bool CheckCollision(GameObject* target, const CollisionParams& collision_params, CollisionParams& hit_collision_params);
	

	/**
	* �I�u�W�F�N�g�̈ʒu��mapchip�����邩�ǂ����`�F�b�N����
	* @param position �I�u�W�F�N�g�̃|�W�V����
	* @return ����̐^�U
	*/
	bool IsHitMapChip(Vector2D position);


public:
	Vector2D stage_size;
protected:
	// �V�[���Ő��������I�u�W�F�N�g
	std::vector<class GameObject*> objects;

	// �ړ��\�I�u�W�F�N�g�z��
	std::vector<class GameObject*> move_objects;

	// �X�N���[�����W�̌��_
	Vector2D screen_offset;

	// �J�������W�̌��_
	Vector2D camera_position;

	class Ground* ground;
};