#pragma once

/**
 * @file	SpriteManager.h
 * @brief	スプライトのマネージャー
**/

#include "Common.h"
#include "TextureManager.h"

#include "WinAPI.h"

// 定数バッファ用 データ構造体 (マテリアル)
struct SpriteConstBufferDataMaterial
{
	Vector4 color;	// 色(RGBA)
	Matrix4 mat;	// 3D変換行列
};

class SpriteManager
{
public:
	// 初期化
	void Init();

	// 描画前処理
	void PreDraw();

public: // メンバ変数
	ComPtr<ID3DBlob> vsBlob_ = nullptr;		 // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob_ = nullptr;		 // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob_ = nullptr;	 // エラーオブジェクト

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

	// パイプラインステートの生成
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;

public:
	static Matrix4 sSpriteProjection_;
};