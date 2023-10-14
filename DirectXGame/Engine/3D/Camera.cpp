/**
 * @file	Camera.cpp
 * @brief	カメラの初期化/更新/描画等の処理
**/

#include "Camera.h"
#include "WinAPI.h"
#include "DirectXBase.h"

Camera* Camera::sCurrentCamera = nullptr;

Camera::~Camera()
{
	constBuffCamera_->Unmap(0, nullptr);	// メモリリークは罪
}

void Camera::Initialize()
{
	SetCurrentCamera(this);

	// 射影変換行列
	matProjection_ = matProjection_.CreateProjectionMat(
		UsaMath::DegreesToRadians(90.0f),	// 上下画角90度
		(float)WinAPI::GetInstance()->width_ / WinAPI::GetInstance()->height_,
		nearZ_, farZ_
	);

	HRESULT result = S_FALSE;

	// ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUの転送用

	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(CameraConstBufferData) + 0xff) & ~0xff; // 256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファの生成
	result = DirectXBase::GetInstance()->device_->CreateCommittedResource(
		&cbHeapProp,	// ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,// リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffCamera_));
	assert(SUCCEEDED(result));

	// 定数バッファのマッピング
	result = constBuffCamera_->Map(0, nullptr, (void**)&constMapCamera_);
	assert(SUCCEEDED(result));
}

void Camera::Initialize(Vector3 position, Vector3 target, Vector3 up)
{
	SetCurrentCamera(this);

	// ベクトルをもらってくる
	position_ = position;
	target_ = target;
	up_ = up;

	// 射影変換行列
	matProjection_ = matProjection_.CreateProjectionMat(
		UsaMath::DegreesToRadians(90.0f),	// 上下画角90度
		(float)WinAPI::GetInstance()->width_ / WinAPI::GetInstance()->height_,
		nearZ_, farZ_
	);

	HRESULT result = S_FALSE;

	// ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUの転送用

	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(CameraConstBufferData) + 0xff) & ~0xff; // 256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファの生成
	result = DirectXBase::GetInstance()->device_->CreateCommittedResource(
		&cbHeapProp,	// ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,// リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffCamera_));
	assert(SUCCEEDED(result));

	// 定数バッファのマッピング
	result = constBuffCamera_->Map(0, nullptr, (void**)&constMapCamera_);
	assert(SUCCEEDED(result));
}

void Camera::Update()
{
	matView_ = Matrix4::Identity();
	matView_ = matView_.CreateViewMat(position_, target_, up_);

	constMapCamera_->projection = matProjection_;
	constMapCamera_->view = matView_;
	constMapCamera_->position = position_;

	matBillboard_ = Matrix4::Inverse(matView_);
	matBillboard_.m[3][0] = 0;
	matBillboard_.m[3][1] = 0;
	matBillboard_.m[3][2] = 0;
	matBillboard_.m[3][3] = 1;

	constMapCamera_->billboard = matBillboard_;
}

void Camera::Set()
{
	// 定数バッファビューをセット [カメラ]
	DirectXBase::GetInstance()->commandList_->SetGraphicsRootConstantBufferView(3, constBuffCamera_->GetGPUVirtualAddress());
}

void Camera::SetCurrentCamera(Camera* current)
{
	sCurrentCamera = current;
}

Camera* Camera::GetCurrentCamera()
{
	return sCurrentCamera;
}