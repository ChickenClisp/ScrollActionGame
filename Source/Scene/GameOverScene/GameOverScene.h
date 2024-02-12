#pragma once

#include "../SceneBase.h"

class GameOverScene : public SceneBase
{
public:
	GameOverScene();
	virtual ~GameOverScene() {}

public:
	//~ Begin SceneBase interface
	virtual void Initialize() override;
	virtual SceneType Update(float delta_seconds) override;
	virtual void Draw() override;
	virtual void Finalize() override;
	virtual SceneType GetSceneType() const override { return SceneType::GAMEOVER_SCENE; }
	//~ End SceneBase interface
};
