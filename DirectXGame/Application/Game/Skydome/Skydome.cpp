#include "Skydome.h"

void Skydome::Initialize()
{
	// ���@���f���ǂݍ���
	skydomeModel = OBJModel::LoadFromOBJ("skydome");

	// ���@�̍s�񏉊���
	skydomeObj.scale = { 1000,1000,1000 };
	skydomeObj.rotation = { 0, 0, 0 };
	skydomeObj.position = { 0, 0, 0 };
	skydomeObj.InitializeObject3D();

	// ���@���f���Ǝ��@�I�u�W�F�N�g��R�Â�
	skydomeObj.objModel = &skydomeModel;
}

void Skydome::Update()
{
	skydomeObj.UpdateObject3D();
}

void Skydome::Draw()
{
	// �I�u�W�F�`��
	skydomeObj.DrawObject3D();
}
