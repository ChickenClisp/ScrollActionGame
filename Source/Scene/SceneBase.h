#pragma once

#include <vector>
#include <string>
#include "../Utility/Vector2D.h"
#include "../GameObject/GameObject.h"
#include "../Utility/HitCollisionDirection.h"

/**
 * シーンタイプ
 */
enum class SceneType : unsigned short
{
	TITLE_SCENE,
	INGAME_SCENE,
	GAMEOVER_SCENE,
};


/**
 * シーンの基底クラス
 */
class SceneBase
{
public:
	SceneBase();
	virtual ~SceneBase();

public:
	/**
	 * 初期化
	 */
	virtual void Initialize();

	/**
	 * 更新
	 * @param	delta_seconds	前フレームとの差分時間(s)
	 * @return	次に遷移するシーンタイプ
	 */
	virtual SceneType Update(float delta_seconds);

	/** 
	 * 描画
	 */
	virtual void Draw();

	/**
	 * 解放
	 */
	virtual void Finalize();

	/**
	 * シーンタイプの取得
	 * @return シーンタイプ
	 */
	virtual SceneType GetSceneType() const = 0;

	/**
	 * GameObjectの生成
	 * Templateで指定したGameObjectクラスを派生したクラスのインスタンス生成する
	 * @param	Position	初期位置
	 * @return	生成したGameObject
	 */
	template <class T>
	T* CreateObject(const Vector2D& position)
	{
		// GameObjectの生成
		T* new_instance = new T();
		GameObject* new_object = dynamic_cast<GameObject*>(new_instance);

		// GameObjectを派生していない場合は、破棄して終了する
		if (new_object == nullptr)
		{
			//派生していないクラスのため、生成しない
			delete new_instance;
			return nullptr;
		}

		// GameObjectの初期化
		new_object->SetOwnerScene(this);
		new_object->SetPosition(position);
		new_object->Initialize();
		objects.push_back(new_object);

		return new_instance;
	}

	/**
	 * GameObjectの破棄
	 * GameObjectを破棄し、配列から削除する
	 * @param	object	破棄するオブジェクト
	 */
	void DestroyObject(class GameObject* object);

	/**
	 * 全てのGameObjectの破棄
	 * シーンに生成されている全てのオブジェクトを破棄する
	 */
	void DestroyAllObjects();

	/**
	 * 衝突判定
	 */
	void UpdateCheckCollision();
	/**
	 * 衝突判定
	 * @param	target 対象オブジェクト
	 *			collision_params　自オブジェクトのコリジョンパラメータ
	 *			hit_collision_params 対象オブジェクトのコリジョンパラメータ
	 * @return	判定の真偽
	 */
	bool CheckCollision(GameObject* target, const CollisionParams& collision_params, CollisionParams& hit_collision_params);
	

	/**
	* オブジェクトの位置にmapchipがあるかどうかチェックする
	* @param position オブジェクトのポジション
	* @return 判定の真偽
	*/
	bool IsHitMapChip(Vector2D position);


public:
	Vector2D stage_size;
protected:
	// シーンで生成したオブジェクト
	std::vector<class GameObject*> objects;

	// 移動可能オブジェクト配列
	std::vector<class GameObject*> move_objects;

	// スクロール座標の原点
	Vector2D screen_offset;

	// カメラ座標の原点
	Vector2D camera_position;

	class Ground* ground;
};