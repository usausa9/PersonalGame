#include "Sprite.h"
#include "DirectXBase.h"
#include "SpriteManager.h"

#include "WinAPI.h"

// ���_�f�[�^�\����
struct Vertex
{
	Vector3 pos;	// xyz���W
	Vector2 uv;	// uv���W
};

Sprite::Sprite()
{
	
}

Sprite::Sprite(TextureIndex tex) : tIndex_(tex)
{
	Init();
}

Sprite::~Sprite()
{
	constBuffMaterial_->Unmap(0, nullptr);	//  ���������[�N�͍�
}

void Sprite::Init()
{
#pragma region ���_�o�b�t�@
	HRESULT pResult;

	float w = TextureManager::GetData(tIndex_)->metadata.width / 2.f;
	float h = TextureManager::GetData(tIndex_)->metadata.height / 2.f;

	// ���_�f�[�^
	Vertex vertices[] =
	{
		// X  Y   Z        U     V
		{{-w,-h, 0.0f },  {0.0f, 0.0f}},	// ����
		{{-w, h, 0.0f },  {0.0f, 1.0f}},	// ����
		{{ w,-h, 0.0f },  {1.0f, 0.0f}},	// �E��
		{{ w, h, 0.0f },  {1.0f, 1.0f}},	// �E��
	};

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	pResult = DirectXBase::GetInstance()->device_->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(pResult));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	pResult = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(pResult));

	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}

	// �q���������
	vertBuff_->Unmap(0, nullptr);
	
	// GPU���z�A�h���X
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView_.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView_.StrideInBytes = sizeof(vertices[0]);
#pragma endregion

#pragma region �萔�o�b�t�@
	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(SpriteConstBufferDataMaterial) + 0xff) & ~0xff; // 256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT vResult;

	// �萔�o�b�t�@�̐���
	vResult = DirectXBase::GetInstance()->device_->CreateCommittedResource(
		&cbHeapProp,	// �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,// ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial_));
	assert(SUCCEEDED(vResult));

	// �萔�o�b�t�@�̃}�b�s���O
	vResult = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial_); // �}�b�s���O
	assert(SUCCEEDED(vResult));

	// �l���������ނƎ����I�ɓ]�������
	constMapMaterial_->color = Vector4{1.f, 0.f, 1.f, 1.f}; // RGBA�Ŕ������̐�
	
	Matrix4 matWorld;
	matWorld = matWorld.Scale({ scale_.x, scale_.y, 1 });
	matWorld *= matWorld.RotateZ(rotation_);
	matWorld *= matWorld.Translate({ position_.x, position_.y, 0 });

	constMapMaterial_->mat = matWorld * SpriteManager::sSpriteProjection_;
	
#pragma endregion
}

void Sprite::Update() 
{
	Matrix4 matWorld;
	matWorld = matWorld.Scale({ scale_.x, scale_.y, 1 });
	matWorld *= matWorld.RotateZ(rotation_);
	matWorld *= matWorld.Translate({ position_.x, position_.y, 0 });

	constMapMaterial_->mat = matWorld * SpriteManager::sSpriteProjection_;
};

void Sprite::Draw()
{
	ID3D12GraphicsCommandList* commandList = DirectXBase::GetInstance()->commandList_.Get();

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	commandList->IASetVertexBuffers(0, 1, &vbView_);

	// CBV�̐ݒ�R�}���h
	commandList->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootDescriptorTable(1, TextureManager::GetData(tIndex_)->gpuHandle);

	// �`��R�}���h
	commandList->DrawInstanced(4, 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
}