#pragma once
#include "UsaFramework.h"

class MyGame : public UsaFramework
{
public:	// �����o�֐�
	// ������
	void Initialize() override;

	// �I��
	void Finalize() override;

	// ���t���[���X�V
	void Update() override;

	// �`��
	void Draw() override;
};