#pragma once
#include "Common.h"
#include "Vector3.h"
#include "OBJModel.h"
#include "Camera.h"

// 定数バッファ用データ構造体 (3D変換行列)
struct ParticleConstBufferDataTransform
{
	Matrix4 mat; // 3D変換行列
};

struct Vertex
{
	Vector3 pos;
	Vector2 scale;
	Vector4 color;
};

struct Particle
{
	Vector3 position = {};	// 座標
	Vector3 velocity = {}; // 速度
	Vector3 accel = {};	// 加速度
	uint32_t frame = 0;			// 現在フレーム
	uint32_t num_frame = 0;		// 終了フレーム
	Vector4 color = { 1,1,1,1 };

	float scale = 1.0f;

	float s_scale = 1.0f;
	float e_scale = 0.0f;

	void Update()
	{
		float t = (float)frame / num_frame;
		float t2 = 1 - t;

		//color = { t2,0,t,t2 };
	};
};

class ParticleManager
{
public:
	// 定数バッファ (行列用)
	ID3D12Resource* constBuffTransform_;

	// 頂点データセット
	const uint16_t VERTEX_COUNT_ = 1024;
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	Vertex* vertMap_ = nullptr;

	// 定数バッファマップ (行列用)
	ParticleConstBufferDataTransform* constMapTransform_;

	// アフィン変換情報
	Vector3 scale_ = { 1,1,1 };
	Vector3 rotation_ = { 0,0,0 };
	Vector3 position_ = { 0,0,0 };

public:
	static ComPtr<ID3D12RootSignature> sRootSignature_; 	// ルートシグネチャ
	static ComPtr<ID3D12PipelineState> sPipelineState_;	// パイプライン

	// パイプライン作成
	static void CreatePipeline();

	// particle初期化
	void InitializeParticle();

	// particle更新処理
	void UpdateParticle();

	// particle描画処理
	void DrawParticle(TextureIndex index);

	// コンテナに追加するもの
	void Add(int life, Vector3 position, Vector3 velocity, Vector3 accel, float start_scale, float end_scale);

private:
	uint16_t activeCount_ = 0;
	forward_list<Particle> particles_;
};