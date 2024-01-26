#include "SceneBase.h"
#include "../GameObject/Ground/Ground.h"
#include "DxLib.h"

SceneBase::SceneBase()
	: screen_offset(Vector2D())
	, camera_position(Vector2D())
	, move_objects()
	, ground(nullptr)
	, stage_size()
{
}

SceneBase::~SceneBase()
{
	Finalize();
}

void SceneBase::Initialize()
{
}

SceneType SceneBase::Update(float delta_seconds)
{
	for (auto iterator = objects.begin(); iterator != objects.end(); ++iterator)
	{
		(*iterator)->Update(delta_seconds);
	}
	UpdateCheckCollision();

	return GetSceneType();
}

void SceneBase::Draw()
{
	for (auto iterator = objects.begin(); iterator != objects.end(); ++iterator)
	{
		(*iterator)->Draw(screen_offset);
	}
}

void SceneBase::Finalize()
{
	// 全てのオブジェクトを破棄
	DestroyAllObjects();
}

void SceneBase::DestroyObject(GameObject* object)
{
	if (object == nullptr)
	{
		return;
	}

	// 削除対象の検索
	auto iterator = objects.begin();
	for (; iterator != objects.end(); ++iterator)
	{
		if ((*iterator) == object)
		{
			break;
		}
	}
	// 見つからなかった場合は抜ける
	if (iterator == objects.end())
	{
		return;
	}

	// 移動可能オブジェクトに関しても同様に
	auto jterator = move_objects.begin();
	for (; jterator != move_objects.end(); ++jterator)
	{
		if ((*jterator) == object)
		{
			break;
		}
	}

	// 削除
	(*iterator)->Finalize();
	objects.erase(iterator);
	move_objects.erase(jterator);
	delete object;
}

void SceneBase::DestroyAllObjects()
{
	for (auto iterator = objects.begin(); iterator != objects.end(); ++iterator)
	{
		(*iterator)->Finalize();
		delete (*iterator);
	}
	objects.clear();
}

void SceneBase::UpdateCheckCollision()
{
	// 移動可能オブジェクトの当たり判定を行う
	for (auto object : move_objects)
	{
		// =====オブジェクト同士の当たり判定=====
		for (auto opponent_object : move_objects)
		{
			if (object == opponent_object)
			{
				continue;
			}
			// 当たり判定
			if (CheckCollision(object, object->GetCollisionParams(), opponent_object->GetCollisionParams()))
			{
				/*
				// プレイヤーと敵が衝突していた場合、ダメージを与える
				if (object->GetCollisionParams().collision_object_type == CollisionObjectType::PLAYER
					&& opponent_object->GetCollisionParams().collision_object_type == CollisionObjectType::ENEMY)
				{
					Character* character = dynamic_cast<Character*>(object);
					Character* opponent_character = dynamic_cast<Character*>(opponent_object);
					opponent_character->ApplyDamage(opponent_character->GetAttackPower(), character);
				}
				*/
				object->OnHitObject();
			}
		}

		// =====マップチップとの当たり判定=====
		CollisionParams object_body_collision_params = object->GetCollisionParams();
		
		// オブジェクトの矩形4辺のx(y)座標
		float left = object_body_collision_params.center_position.x - (object_body_collision_params.box_extent.x / 2 - 2);  //　-1だと当たっている判定になるため
		float right = object_body_collision_params.center_position.x + (object_body_collision_params.box_extent.x / 2 - 2); //　-1だと当たっている判定になるため
		float top = object_body_collision_params.center_position.y - (object_body_collision_params.box_extent.y / 2 - 1);
		float bottom = object_body_collision_params.center_position.y + (object_body_collision_params.box_extent.y / 2 - 1);
		// オブジェクトの矩形4辺のmapchip座標
		int left_chip = left / SIZE_CHIP_WIDTH;
		int right_chip = right / SIZE_CHIP_WIDTH;
		int top_chip = top / SIZE_CHIP_HEIGHT;
		int bottom_chip = bottom / SIZE_CHIP_HEIGHT;

		// オブジェクトの矩形4頂点のmapchip座標
		Vector2D lefttop_chip = Vector2D(left_chip, top_chip);
		Vector2D leftbottom_chip = Vector2D(left_chip, bottom_chip);
		Vector2D righttop_chip = Vector2D(right_chip, top_chip);
		Vector2D rightbottom_chip = Vector2D(right_chip, bottom_chip);

		// 各辺の検索範囲
		Vector2D edge_list[4][2] =
		{ 
			{ leftbottom_chip, rightbottom_chip }, // 下辺
			{ lefttop_chip, righttop_chip },       // 上辺
			{ righttop_chip, rightbottom_chip },   // 右辺
			{ lefttop_chip, leftbottom_chip }      // 左辺
		};

		// オブジェクトの移動ベクトルから判定で用いる辺を決める
		float delta_position_x = object->GetDeltaPosition().x;
		float delta_position_y = object->GetDeltaPosition().y;
		bool is_check_edge[4] =
		{ 
			delta_position_y > 0.0f ? true : false, // 下辺
			delta_position_y < 0.0f ? true : false, // 上辺
			delta_position_x > 0.0f ? true : false, // 右辺
			delta_position_x < 0.0f ? true : false  // 左辺
		};

		// 矩形の上下の判定
		for (int i = 0; i < 2; i++)
		{
			if (is_check_edge[i] == false)
			{
				continue;
			}
			HitCollisionDirection hit_collision_direction = HitCollisionDirection::NOHIT;
			for (int y = edge_list[i][0].y; y <= edge_list[i][1].y; y++)
			{
				for (int x = edge_list[i][0].x; x <= edge_list[i][1].x; x++)
				{
					if (ground != nullptr)
					{
						std::vector<std::vector<int>> ground_data = ground->GetGroundData();
						int chip_id = ground_data[y][x];
						if (chip_id != 0)
						{
							switch (i)
							{
							case 0: // 下辺
								hit_collision_direction = HitCollisionDirection::BOTTOM;
								break;
							case 1: // 上辺
								hit_collision_direction = HitCollisionDirection::TOP;
								break;
							}
						}
					}
				}
				// 押し戻し
				object->OnHitGroundCollision(y * SIZE_CHIP_HEIGHT, hit_collision_direction);
			}
		}

		object_body_collision_params = object->GetCollisionParams();
		
		// オブジェクトの矩形4辺のx(y)座標
		left = object_body_collision_params.center_position.x - (object_body_collision_params.box_extent.x / 2 - 1);
		right = object_body_collision_params.center_position.x + (object_body_collision_params.box_extent.x / 2 - 1);
		top = object_body_collision_params.center_position.y - (object_body_collision_params.box_extent.y / 2 - 2);    //　-1だと当たっている判定になるため
		bottom = object_body_collision_params.center_position.y + (object_body_collision_params.box_extent.y / 2 - 2); //　-1だと当たっている判定になるため
		// オブジェクトの矩形4辺のmapchip座標
		left_chip = left / SIZE_CHIP_WIDTH;
		right_chip = right / SIZE_CHIP_WIDTH;
		top_chip = top / SIZE_CHIP_HEIGHT;
		bottom_chip = bottom / SIZE_CHIP_HEIGHT;

		// オブジェクトの矩形4頂点のmapchip座標
		lefttop_chip = Vector2D(left_chip, top_chip);
		leftbottom_chip = Vector2D(left_chip, bottom_chip);
		righttop_chip = Vector2D(right_chip, top_chip);
		rightbottom_chip = Vector2D(right_chip, bottom_chip);

		// 各辺の検索範囲
		Vector2D _edge_list[4][2] =
		{
			{ leftbottom_chip, rightbottom_chip }, // 下辺
			{ lefttop_chip, righttop_chip },       // 上辺
			{ righttop_chip, rightbottom_chip },   // 右辺
			{ lefttop_chip, leftbottom_chip }      // 左辺
		};
		//矩形の左右判定
		for (int i = 2; i < 4; i++)
		{
			// 使用しない軸は判定しない
			if (is_check_edge[i] == false)
			{
				continue;
			}
			HitCollisionDirection hit_collision_direction = HitCollisionDirection::NOHIT;
			for (int x = _edge_list[i][0].x; x <= _edge_list[i][1].x; x++)
			{
				for (int y = _edge_list[i][0].y; y <= _edge_list[i][1].y; y++)
				{
					if (ground != nullptr)
					{
						std::vector<std::vector<int>> ground_data = ground->GetGroundData();
						int chip_id = ground_data[y][x];
						if (chip_id != 0)
						{
							switch (i)
							{
							case 2: // 右辺
								hit_collision_direction = HitCollisionDirection::RIGHT;
								break;
							case 3: // 左辺
								hit_collision_direction = HitCollisionDirection::LEFT;
								break;
							}
						}
					}
				}
				// 押し戻し
				object->OnHitGroundCollision(x * SIZE_CHIP_WIDTH, hit_collision_direction);
			}
		}

		/*
		// 矩形全体判定
		for (int i = 0; i < 4; i++)
		{
			// 使用しない軸は判定しない
			if (is_check_edge[i] == false)
			{
				continue;
			}
			HitCollisionDirection hit_collision_direction = HitCollisionDirection::NOHIT;
			for (int y = edge_list[i][0].y; y <= edge_list[i][1].y; y++)
			{
				for (int x = edge_list[i][0].x; x <= edge_list[i][1].x; x++)
				{
					if (ground != nullptr)
					{
						std::vector<std::vector<int>> ground_data = ground->GetGroundData();
						int chip_id = ground_data[y][x];
						if (chip_id != 0)
						{
							switch (i)
							{
							case 0: // 下辺
								hit_collision_direction = HitCollisionDirection::BOTTOM;
								break;
							case 1: // 上辺
								hit_collision_direction = HitCollisionDirection::TOP;
								break;
							case 2: // 右辺
								hit_collision_direction = HitCollisionDirection::RIGHT;
								break;
							case 3: // 左辺
								
								hit_collision_direction = HitCollisionDirection::LEFT;
								break;
							}
						}
					}
					if (i == 2 || i == 3)
					{
						object->OnHitGroundCollision(x * SIZE_CHIP_WIDTH, hit_collision_direction);
					}
				}
				if (i == 0 || i == 1)
				{
					object->OnHitGroundCollision(y * SIZE_CHIP_HEIGHT, hit_collision_direction);
				}
			}
		}
		*/
	}
}


bool SceneBase::CheckCollision(GameObject* target, const CollisionParams& collision_params, const CollisionParams& hit_collision_params)
{
	collision_params.collision_object_type; // GROUND = 1, PLAYER = 2, ENEMY = 4
	collision_params.hit_object_types; // bit演算
	collision_params.collision_type; // BLOCK or OVERLAP

	// x軸, y軸の距離を算出
	float distance_x = abs(collision_params.center_position.x - hit_collision_params.center_position.x);
	float distance_y = abs(collision_params.center_position.y - hit_collision_params.center_position.y);

	// 2つの矩形のx軸, y軸のサイズの和/2を算出
	float size_x = (collision_params.box_extent.x + hit_collision_params.box_extent.x) / 2;
	float size_y = (collision_params.box_extent.y + hit_collision_params.box_extent.y) / 2;

	// サイズの和と距離を比較
	if (distance_x < size_x && distance_y < size_y)
	{
		return true;
	}

	return false;
}

bool SceneBase::IsHitMapChip(Vector2D edge_position)
{
	if (ground != nullptr) {
		std::vector<std::vector<int>> ground_data = ground->GetGroundData();
		int chip_id = ground_data[(int)edge_position.y / SIZE_CHIP_HEIGHT][(int)edge_position.x / SIZE_CHIP_WIDTH];
		return (chip_id != 0);
	}
	return false;
}