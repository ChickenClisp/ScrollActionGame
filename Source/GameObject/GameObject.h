#pragma once

#include "../Utility/Vector2D.h"
#include "../Utility/CollisionParams.h"
#include "../Utility/HitCollisionDirection.h"


/**
 * ゲーム内に表示されるオブジェクトの基底クラス
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
	 * シーンの取得
	 * @return owner_scene
	 */
	class SceneBase* GetOwnerScene() const { return owner_scene; }

	/**
	 * シーンのセット
	 * @param	new_owner_scene	セットするシーン
	 */
	void SetOwnerScene(class SceneBase* new_owner_scene);

	/**
	 * Positionの取得
	 * @return	position
	 */
	Vector2D GetPosition() const { return position; }

	/**
	 * Positionのセット
	 * @param	new_position	セットするPosition
	 */
	void SetPosition(const Vector2D& new_position);

	/**
	 * 描画順の取得
	 * @return	draw_sort_priority
	 */
	int GetDrawSortPriority() const { return draw_sort_priority; }

	/**
	 * 描画順のセット
	 * @param	new_priority	セットする描画順
	 */
	void SetDrawSortPriority(int new_priority);

	/**
	 * コリジョンパラメータのセンターポジションの更新
	 * （SetPosition()したあとに使う）
	 */
	void UpdateCollisionParamsCenterPosition(class GameObject* gameobject);

	/**
	 * マップチップとの衝突イベント(押し戻し)
	 * @param hit_mapchip_position   衝突したマップチップの位置
	 *		  hit_collsion_direction 衝突した面
	 */
	virtual void OnHitGroundCollision(float hit_mapchip_position, HitCollisionDirection hit_collsion_direction) {}

	/**
	 * 他オブジェクトとの衝突イベント
	 * @param opponent_gameobject 衝突した他オブジェクト
	 */
	virtual void OnHitObject(class GameObject* opponent_gameobject) {}

	Vector2D GetPrevPosition() const { return prev_position; }
	Vector2D GetDeltaPosition() const { return delta_position; }

	CollisionParams GetCollisionParams() const { return body_collision_params; }
	void SetCollisionParams(CollisionParams& collision_params);

protected:
	// オーナーとなるシーン
	class SceneBase* owner_scene;

	// 位置
	Vector2D position;
	// 1フレーム前の位置
	Vector2D prev_position;
	// フレーム間差分
	Vector2D delta_position;

	// 描画順。数値が小さい順から描画を行う
	int draw_sort_priority;

	// コリジョンパラメータ
	CollisionParams  body_collision_params;

	// positionからbody_collision_params.center_positionまでのベクトル
	Vector2D center_dir;
};