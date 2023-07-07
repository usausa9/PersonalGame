#include "Camera.h"
#include "WinAPI.h"
#include "DirectXBase.h"

Camera::~Camera()
{
	constBuffCamera->Unmap(0, nullptr);	// ���������[�N�͍�
}

void Camera::Initialize()
{
	// �ˉe�ϊ��s��
	matProjection = matProjection.CreateProjectionMat(
		UsaMath::DegreesToRadians(90.0f),	// �㉺��p90�x
		(float)WinAPI::Get()->width / WinAPI::Get()->height,
		0.1f, 1000.0f
	);

	HRESULT result = S_FALSE;

	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(CameraConstBufferData) + 0xff) & ~0xff; // 256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �萔�o�b�t�@�̐���
	result = DirectXBase::Get()->device->CreateCommittedResource(
		&cbHeapProp,	// �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,// ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffCamera));
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�̃}�b�s���O
	result = constBuffCamera->Map(0, nullptr, (void**)&constMapCamera);
	assert(SUCCEEDED(result));
}

void Camera::Update()
{
	matView = Matrix4::Identity();
	matView = matView.CreateViewMat(position, target, up);

	constMapCamera->projection = matProjection;
	constMapCamera->view = matView;
	constMapCamera->position = position;

	matBillboard = Matrix4::Inverse(matView);
	matBillboard.m[3][0] = 0;
	matBillboard.m[3][1] = 0;
	matBillboard.m[3][2] = 0;
	matBillboard.m[3][3] = 1;

	constMapCamera->billboard = matBillboard;
}

void Camera::Set()
{
	// �萔�o�b�t�@�r���[���Z�b�g [�J����]
	DirectXBase::Get()->commandList->SetGraphicsRootConstantBufferView(3, constBuffCamera->GetGPUVirtualAddress());
}