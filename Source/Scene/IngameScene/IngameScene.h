#pragma once

#include "../SceneBase.h"

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
	
private:
	/**
	* csv�t�@�C���̓ǂݍ���
	* @param filename
	*		 ground_data
	*/
	void LoadCSV(const std::string& filename, std::vector<std::vector<int>>& ground_data);
	

private:
	class Player* player;
	int stage_size;
};