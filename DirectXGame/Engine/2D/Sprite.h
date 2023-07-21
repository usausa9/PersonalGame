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
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vbView{}; // ���_�o�b�t�@�r���[�̍쐬

	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;	// �萔�o�b�t�@
	SpriteConstBufferDataMaterial* constMapMaterial = nullptr;// �}�b�v�̊��蓖��

public:
	TextureIndex tIndex = 0;	// �`�悷����̂�������悤�ɂ���

	Vector2 position = { 100,100 };
	Vector2 scale = { 1,1 };
	float rotation = 0;
};