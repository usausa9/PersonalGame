#include "Skydome.h"

void Skydome::Initialize()
{
	// ���@���f���ǂݍ���
	skydomeModel_ = OBJModel::LoadFromOBJ("skydome");

	// ���@�̍s�񏉊���
	skydomeObj_.scale_ = { 1000,1000,1000 };
	skydomeObj_.rotation_ = { 0, 0, 0 };
	skydomeObj_.position_ = { 0, 0, 0 };
	skydomeObj_.InitializeObject3D();

	// ���@���f���Ǝ��@�I�u�W�F�N�g��R�Â�
	skydomeObj_.objModel_ = &skydomeModel_;
}

void Skydome::Update()
{
	skydomeObj_.UpdateObject3D();
}

void Skydome::Draw()
{
	// �I�u�W�F�`��
	skydomeObj_.DrawObject3D();
}
