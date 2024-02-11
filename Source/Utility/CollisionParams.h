#pragma once
#include "Vector2D.h"

/**
 * コリジョンのオブジェクトタイプ
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
 * コリジョンタイプ
 */
enum class CollisonType : unsigned short
{
	BLOCK,
	OVERLAP,
};

/**
 * 矩形コリジョンクラス
 */
struct CollisionParams
{
public:
	// 中心座標
	Vector2D center_position;

	// 矩形の長さ
	Vector2D box_extent;

	// コリジョンのオブジェクトタイプ
	CollisionObjectType collision_object_type;

	// コリジョンタイプ
	CollisonType collision_type;
};