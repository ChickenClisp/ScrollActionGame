#pragma once

#include "../SceneBase.h"

/**
 * サンプルシーン
 * サンプル用に簡易的に実装
 */
class SampleScene : public SceneBase
{
public:
	SampleScene();
	virtual ~SampleScene() {}

public:
	//~ Begin CSceneBase interface
	virtual void Initialize() override;	
	virtual SceneType Update(float delta_seconds) override;
	virtual void Draw() override;
	virtual void Finalize() override;
	virtual SceneType GetSceneType() const override { return SceneType::SAMPLE_SCENE; }
	//~ End SceneBase interface
};