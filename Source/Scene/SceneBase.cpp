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
	// �S�ẴI�u�W�F�N�g��j��
	DestroyAllObjects();
}

void SceneBase::DestroyObject(GameObject* object)
{
	if (object == nullptr)
	{
		return;
	}

	// �폜�Ώۂ̌���
	auto iterator = objects.begin();
	for (; iterator != objects.end(); ++iterator)
	{
		if ((*iterator) == object)
		{
			break;
		}
	}
	// ������Ȃ������ꍇ�͔�����
	if (iterator == objects.end())
	{
		return;
	}

	// �ړ��\�I�u�W�F�N�g�Ɋւ��Ă����l��
	auto jterator = move_objects.begin();
	for (; jterator != move_objects.end(); ++jterator)
	{
		if ((*jterator) == object)
		{
			break;
		}
	}

	// �폜
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
	// �ړ��\�I�u�W�F�N�g�̓����蔻����s��
	for (auto object : move_objects)
	{
		// =====�I�u�W�F�N�g���m�̓����蔻��=====
		for (auto opponent_object : move_objects)
		{
			if (object == opponent_object)
			{
				continue;
			}
			// �����蔻��
			if (CheckCollision(object, object->GetCollisionParams(), opponent_object->GetCollisionParams()))
			{
				/*
				// �v���C���[�ƓG���Փ˂��Ă����ꍇ�A�_���[�W��^����
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

		// =====�}�b�v�`�b�v�Ƃ̓����蔻��=====
		CollisionParams object_body_collision_params = object->GetCollisionParams();
		
		// �I�u�W�F�N�g�̋�`4�ӂ�x(y)���W
		float left = object_body_collision_params.center_position.x - (object_body_collision_params.box_extent.x / 2 - 2);  //�@-1���Ɠ������Ă��锻��ɂȂ邽��
		float right = object_body_collision_params.center_position.x + (object_body_collision_params.box_extent.x / 2 - 2); //�@-1���Ɠ������Ă��锻��ɂȂ邽��
		float top = object_body_collision_params.center_position.y - (object_body_collision_params.box_extent.y / 2 - 1);
		float bottom = object_body_collision_params.center_position.y + (object_body_collision_params.box_extent.y / 2 - 1);
		// �I�u�W�F�N�g�̋�`4�ӂ�mapchip���W
		int left_chip = left / SIZE_CHIP_WIDTH;
		int right_chip = right / SIZE_CHIP_WIDTH;
		int top_chip = top / SIZE_CHIP_HEIGHT;
		int bottom_chip = bottom / SIZE_CHIP_HEIGHT;

		// �I�u�W�F�N�g�̋�`4���_��mapchip���W
		Vector2D lefttop_chip = Vector2D(left_chip, top_chip);
		Vector2D leftbottom_chip = Vector2D(left_chip, bottom_chip);
		Vector2D righttop_chip = Vector2D(right_chip, top_chip);
		Vector2D rightbottom_chip = Vector2D(right_chip, bottom_chip);

		// �e�ӂ̌����͈�
		Vector2D edge_list[4][2] =
		{ 
			{ leftbottom_chip, rightbottom_chip }, // ����
			{ lefttop_chip, righttop_chip },       // ���
			{ righttop_chip, rightbottom_chip },   // �E��
			{ lefttop_chip, leftbottom_chip }      // ����
		};

		// �I�u�W�F�N�g�̈ړ��x�N�g�����画��ŗp����ӂ����߂�
		float delta_position_x = object->GetDeltaPosition().x;
		float delta_position_y = object->GetDeltaPosition().y;
		bool is_check_edge[4] =
		{ 
			delta_position_y > 0.0f ? true : false, // ����
			delta_position_y < 0.0f ? true : false, // ���
			delta_position_x > 0.0f ? true : false, // �E��
			delta_position_x < 0.0f ? true : false  // ����
		};

		// ��`�̏㉺�̔���
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
							case 0: // ����
								hit_collision_direction = HitCollisionDirection::BOTTOM;
								break;
							case 1: // ���
								hit_collision_direction = HitCollisionDirection::TOP;
								break;
							}
						}
					}
				}
				// �����߂�
				object->OnHitGroundCollision(y * SIZE_CHIP_HEIGHT, hit_collision_direction);
			}
		}

		object_body_collision_params = object->GetCollisionParams();
		
		// �I�u�W�F�N�g�̋�`4�ӂ�x(y)���W
		left = object_body_collision_params.center_position.x - (object_body_collision_params.box_extent.x / 2 - 1);
		right = object_body_collision_params.center_position.x + (object_body_collision_params.box_extent.x / 2 - 1);
		top = object_body_collision_params.center_position.y - (object_body_collision_params.box_extent.y / 2 - 2);    //�@-1���Ɠ������Ă��锻��ɂȂ邽��
		bottom = object_body_collision_params.center_position.y + (object_body_collision_params.box_extent.y / 2 - 2); //�@-1���Ɠ������Ă��锻��ɂȂ邽��
		// �I�u�W�F�N�g�̋�`4�ӂ�mapchip���W
		left_chip = left / SIZE_CHIP_WIDTH;
		right_chip = right / SIZE_CHIP_WIDTH;
		top_chip = top / SIZE_CHIP_HEIGHT;
		bottom_chip = bottom / SIZE_CHIP_HEIGHT;

		// �I�u�W�F�N�g�̋�`4���_��mapchip���W
		lefttop_chip = Vector2D(left_chip, top_chip);
		leftbottom_chip = Vector2D(left_chip, bottom_chip);
		righttop_chip = Vector2D(right_chip, top_chip);
		rightbottom_chip = Vector2D(right_chip, bottom_chip);

		// �e�ӂ̌����͈�
		Vector2D _edge_list[4][2] =
		{
			{ leftbottom_chip, rightbottom_chip }, // ����
			{ lefttop_chip, righttop_chip },       // ���
			{ righttop_chip, rightbottom_chip },   // �E��
			{ lefttop_chip, leftbottom_chip }      // ����
		};
		//��`�̍��E����
		for (int i = 2; i < 4; i++)
		{
			// �g�p���Ȃ����͔��肵�Ȃ�
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
							case 2: // �E��
								hit_collision_direction = HitCollisionDirection::RIGHT;
								break;
							case 3: // ����
								hit_collision_direction = HitCollisionDirection::LEFT;
								break;
							}
						}
					}
				}
				// �����߂�
				object->OnHitGroundCollision(x * SIZE_CHIP_WIDTH, hit_collision_direction);
			}
		}

		/*
		// ��`�S�̔���
		for (int i = 0; i < 4; i++)
		{
			// �g�p���Ȃ����͔��肵�Ȃ�
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
							case 0: // ����
								hit_collision_direction = HitCollisionDirection::BOTTOM;
								break;
							case 1: // ���
								hit_collision_direction = HitCollisionDirection::TOP;
								break;
							case 2: // �E��
								hit_collision_direction = HitCollisionDirection::RIGHT;
								break;
							case 3: // ����
								
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
	collision_params.hit_object_types; // bit���Z
	collision_params.collision_type; // BLOCK or OVERLAP

	// x��, y���̋������Z�o
	float distance_x = abs(collision_params.center_position.x - hit_collision_params.center_position.x);
	float distance_y = abs(collision_params.center_position.y - hit_collision_params.center_position.y);

	// 2�̋�`��x��, y���̃T�C�Y�̘a/2���Z�o
	float size_x = (collision_params.box_extent.x + hit_collision_params.box_extent.x) / 2;
	float size_y = (collision_params.box_extent.y + hit_collision_params.box_extent.y) / 2;

	// �T�C�Y�̘a�Ƌ������r
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