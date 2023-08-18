#include "Object3D.h"

#include "DirectXBase.h"
#include "BaseCollider.h"

Camera* Object3D::camera_ = nullptr;

Object3D::~Object3D()
{
	if (collider_)
	{
		// �Փ˃}�l�[�W������o�^����
		CollisionManager::GetInstance()->RemoveCollider(collider_);
		delete collider_;
	}
}

void Object3D::InitializeObject3D()
{
	HRESULT result;

	// �萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// �萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(constMapTransform_) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �萔�o�b�t�@�̐���
	result = DirectXBase::GetInstance()->device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform_));
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);
	assert(SUCCEEDED(result));

	// �N���X���̕�������擾
	name_ = typeid(*this).name();
}

// 3D�I�u�W�F�N�g�X�V����
void Object3D::UpdateObject3D()
{
	Matrix4 matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = matScale.Scale(scale_);
	matRot = Matrix4::Identity();
	matRot *= matRot.RotateZ(rotation_.z);
	matRot *= matRot.RotateX(rotation_.x);
	matRot *= matRot.RotateY(rotation_.y);
	matTrans = matTrans.Translate(position_);

	// ���[���h�s��̍���
	matWorld_ = Matrix4::Identity();	// �ό`�����Z�b�g
	matWorld_ *= matScale;			// ���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld_ *= matRot;				// ���[���h�s��ɉ�]�𔽉f
	matWorld_ *= matTrans;			// ���[���h�s��ɕ��s�ړ��𔽉f

	// �e�I�u�W�F�N�g�������
	if (parent_ != nullptr)
	{
		// �e�I�u�W�F�N�g�̃��[���h�s����|����
		matWorld_ *= parent_->matWorld_;
	}

	// �萔�o�b�t�@�փf�[�^�]��
	constMapTransform_->mat = matWorld_;

	// �����蔻��X�V
	if (collider_)
	{
		collider_->Update();
	}
}

void Object3D::DrawObject3D()
{
	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	DirectXBase::GetInstance()->commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());
	
	// �`��I
	objModel_->Draw();
}

void Object3D::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);
	this->collider_ = collider;

	// �Փ˃}�l�[�W���ɓo�^
	CollisionManager::GetInstance()->AddCollider(collider);

	// �R���C�_�[���X�V
	collider->Update();
}