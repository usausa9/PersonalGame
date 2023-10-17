/**
 * @file	TextureManager.cpp
 * @brief	テクスチャのマネージャー
**/

#include "TextureManager.h"
#include "DirectXBase.h"

uint16_t TextureManager::sSrvIndex_ = 0;
D3D12_DESCRIPTOR_HEAP_DESC TextureManager::sSrvHeapDesc_{};
ComPtr<ID3D12DescriptorHeap> TextureManager::sSrvHeap_ = nullptr;
ComPtr<ID3D12Resource> TextureManager::sTexBuff_[s_MAX_TEXTURE_NUM_] = {};
TextureData TextureManager::sTextureData_[s_MAX_TEXTURE_NUM_] = {};

void TextureManager::Init()
{
	HRESULT result = S_FALSE;

	sSrvIndex_ = 0;

	// デスクリプタヒープの設定
	sSrvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	sSrvHeapDesc_.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// シェーダーから見えるように
	sSrvHeapDesc_.NumDescriptors = s_MAX_TEXTURE_NUM_;

	// 設定をもとにSRV用デスクリプタヒープを生成
	result = DirectXBase::GetInstance()->device_->CreateDescriptorHeap(&sSrvHeapDesc_, IID_PPV_ARGS(&sSrvHeap_));
	assert(SUCCEEDED(result));

	for (auto& buff : sTexBuff_)
	{
		buff = nullptr;
	}

	for (auto& data : sTextureData_)
	{
		data = {};
	}
}

TextureIndex TextureManager::Load(std::wstring filepath)
{
	HRESULT result = S_FALSE;

	if (sSrvIndex_ >= s_MAX_TEXTURE_NUM_)
	{
		OutputDebugString(L"over maxTextureNum");

		return static_cast<TextureIndex>(-1); // GG
	}

	ScratchImage scratchImg{};
	// WICテクスチャのロード
	result = LoadFromWICFile(
		filepath.c_str(),
		WIC_FLAGS_NONE,
		&sTextureData_[sSrvIndex_].metadata, scratchImg);

	ScratchImage mipChain{};
	// ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		sTextureData_[sSrvIndex_].metadata = scratchImg.GetMetadata();
	}

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	sTextureData_[sSrvIndex_].metadata.format = MakeSRGB(sTextureData_[sSrvIndex_].metadata.format);

	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM; // GPUの転送用
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = sTextureData_[sSrvIndex_].metadata.format;
	textureResourceDesc.Width = sTextureData_[sSrvIndex_].metadata.width;
	textureResourceDesc.Height = (UINT)sTextureData_[sSrvIndex_].metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)sTextureData_[sSrvIndex_].metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)sTextureData_[sSrvIndex_].metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

#pragma region テクスチャバッファ

	// テクスチャバッファの生成
	result = DirectXBase::GetInstance()->device_->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&sTexBuff_[sSrvIndex_]));
	assert(SUCCEEDED(result));

	// 全ミップマップについて
	for (size_t i = 0; i < sTextureData_[sSrvIndex_].metadata.mipLevels; i++)
	{
		// ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);

		// テクスチャバッファにデータ転送
		result = sTexBuff_[sSrvIndex_]->WriteToSubresource(
			(UINT)i,
			nullptr,		// 全領域へコピー
			img->pixels,	// 元データアドレス
			(UINT)img->rowPitch,	// 1ラインサイズ
			(UINT)img->slicePitch	// 全サイズ
		);
		assert(SUCCEEDED(result));
	}

	// SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = sSrvHeap_->GetCPUDescriptorHandleForHeapStart();

	srvHandle.ptr +=
		DirectXBase::GetInstance()->device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * sSrvIndex_;

	// シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};		// 設定構造体
	srvDesc.Format = textureResourceDesc.Format;// RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	// ハンドルの指す位置にシェーダーリソースビュー作成
	DirectXBase::GetInstance()->device_->CreateShaderResourceView(sTexBuff_[sSrvIndex_].Get(), &srvDesc, srvHandle);

	// GPU用
	D3D12_GPU_DESCRIPTOR_HANDLE gpuSrvHandle = sSrvHeap_->GetGPUDescriptorHandleForHeapStart();

	gpuSrvHandle.ptr +=
		DirectXBase::GetInstance()->device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * sSrvIndex_;

	sTextureData_[sSrvIndex_].cpuHandle = srvHandle;
	sTextureData_[sSrvIndex_].gpuHandle = gpuSrvHandle;

	sSrvIndex_++;

	if (sSrvIndex_ >= 1022)
	{
		sSrvIndex_ = 1;
	}

	return sSrvIndex_ - 1;
}

TextureIndex TextureManager::Load(std::string filepath)
{
	// SJIS → wstring
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str()
		, -1, (wchar_t*)NULL, 0);

	// バッファの取得
	wchar_t* cpUCS2 = new wchar_t[iBufferSize];

	// SJIS → wstring
	MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, cpUCS2
		, iBufferSize);

	// stringの生成
	std::wstring wPath(cpUCS2, cpUCS2 + iBufferSize - 1);

	// バッファの破棄
	delete[] cpUCS2;

	return Load(wPath);
}

TextureData* TextureManager::GetData(TextureIndex index)
{
	return &sTextureData_[index];
}

ID3D12Resource* TextureManager::GetBuff(TextureIndex index)
{
	return sTexBuff_[index].Get();
}

void TextureManager::Release()
{
	for (auto& buff : sTexBuff_)
	{
		if (buff.Get())
		{
			buff = ComPtr<ID3D12Resource>();
		}
	}

	for (auto& data : sTextureData_)
	{
		data = {};
	}
}