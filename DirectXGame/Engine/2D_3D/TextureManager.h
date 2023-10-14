#pragma once
#include "Common.h"
#include <DirectXTex.h>

#include <string>

typedef unsigned int TextureIndex;

struct TextureData
{
	TexMetadata metadata{};

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
};

class TextureManager
{
public:
	// 初期化
	static void Init();

	// テクスチャロード
	static TextureIndex Load(std::wstring filepath);
	static TextureIndex Load(std::string filepath);

	// データ取得
	static TextureData* GetData(TextureIndex index);
	// バッファ取得
	static ID3D12Resource* GetBuff(TextureIndex index);
	// リリース
	static void Release();

private:
	const static uint16_t s_MAX_TEXTURE_NUM_ = 1024;

	static uint16_t sSrvIndex_;
	static D3D12_DESCRIPTOR_HEAP_DESC sSrvHeapDesc_;

	static ComPtr<ID3D12Resource> sTexBuff_[s_MAX_TEXTURE_NUM_];
	static TextureData sTextureData_[s_MAX_TEXTURE_NUM_];

public:
	static ComPtr<ID3D12DescriptorHeap> sSrvHeap_;
};