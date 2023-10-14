#pragma once

/**
 * @file	SceneFactory.h
 * @brief	ゲームのシーン工場
**/

#include "AbstractSceneFactory.h"

class SceneFactory : public AbstractSceneFactory
{
public:
	// シーンを作る関数
	IScene* CreateScene(const std::string& sceneName) override;
};