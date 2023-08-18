#pragma once

#include "IScene.h"
#include <string>

// �V�[���H�� (�T�O)
class AbstractSceneFactory
{
public:
	// ���z�f�X�g���N�^
	virtual ~AbstractSceneFactory() = default;

	// �V�[������
	virtual IScene* CreateScene(const std::string& sceneName) = 0;
};