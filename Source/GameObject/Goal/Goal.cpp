#include "Goal.h"
#include "DxLib.h"

Goal::Goal()
{
}

Goal::~Goal()
{
}

void Goal::Initialize()
{
	center_dir = Vector2D{ 15, 15 };
	body_collision_params = { Vector2D{GetPosition() + center_dir }, Vector2D{30, 30}, CollisionObjectType::ITEM , CollisonType::OVERLAP };

}

void Goal::Update(float delta_seconds)
{
}

void Goal::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);
	// �X�N���[�����W�ɕϊ����ĕ`��
	int x, y, screen_offset_x, screen_offset_y;
	GetPosition().ToInt(x, y);
	screen_offset.ToInt(screen_offset_x, screen_offset_y);
	// �f�o�b�O�p�@�R���W�����̕\��
		DrawBox(body_collision_params.center_position.x - (body_collision_params.box_extent.x / 2 - 1) - screen_offset_x,
		body_collision_params.center_position.y - (body_collision_params.box_extent.y / 2 - 1) - screen_offset_y,
		body_collision_params.center_position.x + (body_collision_params.box_extent.x / 2 - 1) - screen_offset_x,
		body_collision_params.center_position.y + (body_collision_params.box_extent.y / 2 - 1) - screen_offset_y,
		GetColor(255, 255, 0),
		false);
}

void Goal::Finalize()
{
}