#pragma once

#include "../SceneBase.h"
#include "../Source/GameObject/Character/EnemyBase/EnemyBase.h"

/**
 * サンプルシーン
 * サンプル用に簡易的に実装
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
	 * プレイヤーと敵の距離がサーチ範囲内にあるかどうか
	 * @param enemy_base
	 */
	bool IsFoundPlayer(EnemyBase* enemy_base);
	/**
	 * 敵->プレイヤーの方向を返す
	 * @param enemy_base
	 */
	Direction VectorEnemytoPlayer(EnemyBase* enemy_base);
	/**
	 * プレイヤーの攻撃イベント
	 * @param enemy_base
	 */
	void PlayertoEnemyAttackEvent(EnemyBase* enemy_base);
	/**
	 * 敵の攻撃イベント
	 * @param enemy_base
	 */
	void EnemytoPlayerAttackEvent(EnemyBase* enemy_base);
	/**
	 * プレイヤーの死亡イベント
	 */
	void OnPlayerDead();
	/**
	 * プレイヤーのゴールイベント
	 */
	void OnPlayerGoalReached();

private:
	/**
	* csvファイルの読み込み
	* @param filename
	*		 ground_data
	*/
	void LoadCSV(const std::string& filename, std::vector<std::vector<int>>& ground_data);
	/**
	* ステージ初期化
	*/
	void InitStage();

private:
	class Player* player;
	bool is_goal;
	int player_life;
};