#pragma once

#include "AbstractSceneFactory.h"

// このゲーム用のシーン工場
class SceneFactory : public AbstractSceneFactory
{
public:
	// シーンを作る関数
	IScene* CreateScene(const std::string& sceneName) override;
};