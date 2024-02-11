#include "GameObject.h"
#include "../Scene/SceneBase.h"
#include <cmath>

GameObject::GameObject()
	: owner_scene(nullptr)
	, position(Vector2D())
	, draw_sort_priority(0)
	, body_collision_params()
	, center_dir()
	, graphic_handle(0)
	, graphic_handles_map({})
	, animtype()
	, animation_frame(0)
	, animation_frame_adjust(0)
	, animation_speed(1) // animation_frame_adjust���傫������,�N������UpdateAnimation��if����false�ɂȂ�悤�ɂ���
	, animation_is_loop()
{
}

void GameObject::UpdateAnimation()
{
	// animation_frame���Ōォ���[�v���Ȃ��A�j���[�V�����̏ꍇ�A�������Ȃ�
	if (animation_frame == graphic_handles_map[animtype].size() - 1 && animation_is_loop == false)
	{
		return;
	}
	// �ݒ肵��speed�𒴂����Ƃ��Ɏ���animation�ɕς���
	if (++animation_frame_adjust > animation_speed)
	{
		animation_frame++;
		animation_frame_adjust = 0;
		animation_frame %= graphic_handles_map[animtype].size();
	}
}
void GameObject::SetAnimation(AnimType new_animtype, int new_animation_speed, bool b_loop)
{
	animtype = new_animtype;
	graphic_handle = graphic_handles_map[animtype][animation_frame];
	animation_speed = std::floor(60 / new_animation_speed);
	animation_is_loop = b_loop;
}

void GameObject::SetFrameZero()
{
	animation_frame = 0;
	animation_frame_adjust = 0;
}


void GameObject::SetOwnerScene(SceneBase* new_owner_scene)
{
	owner_scene = new_owner_scene;
}

void GameObject::SetPosition(const Vector2D& new_position)
{
	position = new_position;
}

void GameObject::SetDrawSortPriority(int new_priority)
{
	draw_sort_priority = new_priority;
}

void GameObject::UpdateCollisionParamsCenterPosition(class GameObject* gameobject)
{
	// �R���W�����p�����[�^�̍X�V
	gameobject->body_collision_params.center_position = gameobject->GetPosition() + gameobject->center_dir;
}

void GameObject::SetCollisionParams(CollisionParams& collision_params)
{
	body_collision_params = collision_params;
}