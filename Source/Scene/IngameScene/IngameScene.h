#pragma once

#include "../SceneBase.h"
#include "../Source/GameObject/Character/EnemyBase/EnemyBase.h"

/**
 * �T���v���V�[��
 * �T���v���p�ɊȈՓI�Ɏ���
 */
class IngameScene : public SceneBase
{
public:
	IngameScene();
	virtual ~IngameScene() {}

public:
	//~ Begin SceneBase interface
	virtual void Initialize() override;	
	virtual SceneType Update(float delta_seconds) override;
	virtual void Draw() override;
	virtual void Finalize() override;
	virtual SceneType GetSceneType() const override { return SceneType::INGAME_SCENE; }
	//~ End SceneBase interface
	/**
	 * �v���C���[�ƓG�̋������T�[�`�͈͓��ɂ��邩�ǂ���
	 * @param enemy_base
	 */
	bool IsFoundPlayer(EnemyBase* enemy_base);
	/**
	 * �G->�v���C���[�̕�����Ԃ�
	 * @param enemy_base
	 */
	Direction VectorEnemytoPlayer(EnemyBase* enemy_base);
	/**
	 * �v���C���[�̍U���C�x���g
	 * @param enemy_base
	 */
	void PlayertoEnemyAttackEvent(EnemyBase* enemy_base);
	/**
	 * �G�̍U���C�x���g
	 * @param enemy_base
	 */
	void EnemytoPlayerAttackEvent(EnemyBase* enemy_base);
	/**
	 * �v���C���[�̎��S�C�x���g
	 */
	void OnPlayerDead();
	/**
	 * �v���C���[�̃S�[���C�x���g
	 */
	void OnPlayerGoalReached();

private:
	/**
	* csv�t�@�C���̓ǂݍ���
	* @param filename
	*		 ground_data
	*/
	void LoadCSV(const std::string& filename, std::vector<std::vector<int>>& ground_data);
	/**
	* �X�e�[�W������
	*/
	void InitStage();

private:
	class Player* player;
	bool is_goal;
	int player_life;
};