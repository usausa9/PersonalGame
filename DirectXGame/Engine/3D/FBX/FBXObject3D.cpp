#include "FBXObject3D.h"

Camera* FBXObject3D::camera_ = nullptr;

using namespace Microsoft::WRL;
using namespace DirectX;

ID3D12GraphicsCommandList* FBXObject3D::commandList_ = nullptr;
ComPtr<ID3D12RootSignature> FBXObject3D::rootSigunature_;
ComPtr<ID3D12PipelineState> FBXObject3D::pipelineState_;

void FBXObject3D::Initialize()
{
	// 1フレーム分の時間を60FPSで設定
	frameTime_.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

	HRESULT result = S_FALSE;
	// ヒープ設定/リソース設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;	// 256バイトアラインメント
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファの生成
	result = DirectXBase::GetInstance()->device_->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform_));

	// ヒープ設定/リソース設定 Widthのみ書き換え
	resDesc.Width = (sizeof(ConstBufferDataSkin) + 0xff) & ~0xff;	// 256バイトアラインメント

	// 定数バッファの生成 (スキン)
	result = DirectXBase::GetInstance()->device_->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin_));
}

void FBXObject3D::CreateGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob;	// 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob;	// エラーオブジェクト

	assert(DirectXBase::GetInstance()->device_);
	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/FBXVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/FBXPS.hlsl",    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ // xy座標
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 影響を受けるボーン番号(4つ)
			"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 影響を受けるボーン番号(4つ)
			"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	// 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1;	// 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV{};
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	// t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[3] = {};
	// CBV（座標変換行列用）
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ）
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	// CBV (スキニング用)
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = DirectXBase::GetInstance()->device_->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootSigunature_.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootSigunature_.Get();

	// グラフィックスパイプラインの生成
	result = DirectXBase::GetInstance()->device_->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(pipelineState_.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }
}

void FBXObject3D::Update()
{
	// アニメーション用
	if (isPlay_)
	{
		// 1フレーム進める
		currentTime_ += frameTime_;
		// 最後まで再生したら先頭に戻す
		if (currentTime_ > endTime_)
		{
			currentTime_ = startTime_;
		}
	}

	// スケール、回転、平行移動行列の計算
	Matrix4 matScale, matRot, matTrans;

	matScale = Matrix4::Identity();
	matScale.Scale(scale_);

	matRot = Matrix4::Identity();
	matRot *= matRot.RotateZ(rotation_.z);
	matRot *= matRot.RotateX(rotation_.x);
	matRot *= matRot.RotateY(rotation_.y);

	matTrans = Matrix4::Identity();
	matTrans.Translate(position_);

	// ワールド行列合成
	matWorld_ = Matrix4::Identity();
	matWorld_ *= matScale;
	matWorld_ *= matRot;
	matWorld_ *= matTrans;

	// ビュープロジェクション行列
	const Matrix4& matViewProjection = camera_->GetViewProjection();
	// モデルのメッシュトランスフォーム
	const Matrix4& modelTransform = model_->GetModelTransform();
	// カメラ座標
	const Vector3& cameraPos = camera_->GetEye();

	HRESULT result = S_FALSE;
	// 定数バッファへデータ転送
	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform_->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->viewProjection_ = matViewProjection;
		constMap->worldTransform_ = modelTransform * matWorld_;
		constMap->cameraPosition_ = cameraPos;
		constBuffTransform_->Unmap(0, nullptr);
	}

	// ボーン配列
	std::vector<FBXModel::Bone>& bones = model_->GetBones();

	// 定数バッファへデータ転送
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin_->Map(0, nullptr, (void**)&constMapSkin);
	for (uint32_t i = 0; i < bones.size(); i++)
	{
		// 今の姿勢行列
		Matrix4 matCurrentPose;
		// 今の姿勢行列を取得
		FbxAMatrix fbxCurrentPose =
			bones[i].fbxCluster_->GetLink()->EvaluateGlobalTransform(currentTime_);
		// Matrix4に変換
		FBXLoader::ConvertMatrixFromFBX(&matCurrentPose, fbxCurrentPose);
		// 合成してスキニング行列に
		constMapSkin->bones_[i] = bones[i].invInitialPose_ * matCurrentPose;
	}
	constBuffSkin_->Unmap(0, nullptr);
}

void FBXObject3D::Draw()
{
	// モデルがなければ描画しない
	if (model_ == nullptr)
	{
		return;
	}

	// パイプラインステートの設定
	commandList_->SetPipelineState(pipelineState_.Get());
	// ルートシグネチャの設定
	commandList_->SetGraphicsRootSignature(rootSigunature_.Get());
	// プリミティブ形状を設定
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// 定数バッファビューをセット
	commandList_->SetGraphicsRootConstantBufferView(0, constBuffTransform_->GetGPUVirtualAddress());
	// 定数バッファビューをセット (スキン)
	commandList_->SetGraphicsRootConstantBufferView(2, constBuffSkin_->GetGPUVirtualAddress());

	// モデル描画
	model_->Draw(commandList_);
}

void FBXObject3D::PlayAnimation()
{
	FbxScene* fbxScene = model_->GetFbxScene();
	//0番のアニメーション取得
	FbxAnimStack* animStack = fbxScene->GetSrcObject<FbxAnimStack>(0);
	// アニメーションの名前取得
	const char* animStackName = animStack->GetName();
	// アニメーションの時間取得
	FbxTakeInfo* takeInfo = fbxScene->GetTakeInfo(animStackName);

	// 開始時間取得
	startTime_ = takeInfo->mLocalTimeSpan.GetStart();
	// 終了時間取得
	endTime_ = takeInfo->mLocalTimeSpan.GetStop();
	// 開始時間に合わせる
	currentTime_ = startTime_;
	// 再生中状態にする
	isPlay_ = true;
}
