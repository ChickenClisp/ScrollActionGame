#pragma once

#include "../SceneBase.h"

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
	
private:
	/**
	* csvファイルの読み込み
	* @param filename
	*		 ground_data
	*/
	void LoadCSV(const std::string& filename, std::vector<std::vector<int>>& ground_data);
	

private:
	class Player* player;
	int stage_size;
};