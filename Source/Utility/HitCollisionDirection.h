#pragma once

/*
* コリジョンが当たった方向
*/
enum class HitCollisionDirection : unsigned short
{
	TOP,
	BOTTOM,
	RIGHT,
	LEFT,
	NOHIT
};