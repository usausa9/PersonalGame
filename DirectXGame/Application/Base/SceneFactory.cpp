/**
 * @file	SceneFactory.cpp
 * @brief	ゲームのシーン工場
**/

#include "SceneFactory.h"

#include "TitleScene.h"
#include "GameScene.h"

IScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	// 次のシーンを生成
	IScene* newScene = nullptr;

	if (sceneName == "TITLE")
	{
		newScene = new TitleScene();
	}
	else if (sceneName == "GAME")
	{
		newScene = new GameScene();
	}

	return newScene;
}