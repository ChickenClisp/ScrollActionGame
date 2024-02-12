#pragma once

#include "../SceneBase.h"

class TitleScene : public SceneBase
{
public:
	TitleScene();
	virtual ~TitleScene() {}

public:
	//~ Begin SceneBase interface
	virtual void Initialize() override;
	virtual SceneType Update(float delta_seconds) override;
	virtual void Draw() override;
	virtual void Finalize() override;
	virtual SceneType GetSceneType() const override { return SceneType::TITLE_SCENE; }
	//~ End SceneBase interface
};
