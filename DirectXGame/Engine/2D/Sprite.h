#pragma once
#include "Common.h"
#include "SpriteManager.h"

class Sprite
{
public:
	Sprite();
	Sprite(TextureIndex tex);
	~Sprite();

private:
	// ������
	void Init();

public:
	void Update();
	void Draw();

public: // �����o�ϐ�
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vbView_{}; // ���_�o�b�t�@�r���[�̍쐬

	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;	// �萔�o�b�t�@
	SpriteConstBufferDataMaterial* constMapMaterial_ = nullptr;// �}�b�v�̊��蓖��

public:
	TextureIndex tIndex_ = 0;	// �`�悷����̂�������悤�ɂ���

	Vector2 position_ = { 100,100 };
	Vector2 scale_ = { 1,1 };
	float rotation_ = 0;
};