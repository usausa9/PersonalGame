/**
 * @file	Object3D.cpp
 * @brief	OBJモデルのオブジェクトを初期化/更新/描画
**/

#include "Object3D.h"

#include "DirectXBase.h"
#include "BaseCollider.h"

Camera* Object3D::sCamera_ = nullptr;

Object3D::~Object3D()
{
	if (collider_)
	{
		// 衝突マネージャから登録解除
		CollisionManager::GetInstance()->RemoveCollider(collider_);
		delete collider_;
	}
}

void Object3D::InitializeObject3D()
{
	HRESULT result;

	// 定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// 定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(constMapTransform_) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファの生成
	result = DirectXBase::GetInstance()->device_->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform_));
	assert(SUCCEEDED(result));

	// 定数バッファのマッピング
	result = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);
	assert(SUCCEEDED(result));

	// クラス名の文字列を取得
	NAME_ = typeid(*this).name();
}

// 3Dオブジェクト更新処理
void Object3D::UpdateObject3D()
{
	Matrix4 matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = matScale.Scale(scale_);
	matRot = Matrix4::Identity();
	matRot *= matRot.RotateZ(rotation_.z);
	matRot *= matRot.RotateX(rotation_.x);
	matRot *= matRot.RotateY(rotation_.y);
	matTrans = matTrans.Translate(position_);

	// ワールド行列の合成
	matWorld_ = Matrix4::Identity();	// 変形をリセット
	matWorld_ *= matScale;			// ワールド行列にスケーリングを反映
	matWorld_ *= matRot;				// ワールド行列に回転を反映
	matWorld_ *= matTrans;			// ワールド行列に平行移動を反映

	// 親オブジェクトがあれば
	if (parent_ != nullptr)
	{
		// 親オブジェクトのワールド行列を掛ける
		matWorld_ *= parent_->matWorld_;
	}

	// 定数バッファへデータ転送
	constMapTransform_->mat = matWorld_;

	// 当たり判定更新
	if (collider_)
	{
		collider_->Update();
	}
}

void Object3D::DrawObject3D()
{
	// 定数バッファビュー(CBV)の設定コマンド
	DirectXBase::GetInstance()->commandList_->SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	// 描画！
	objModel_->Draw();
}

void Object3D::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);
	this->collider_ = collider;

	// 衝突マネージャに登録
	CollisionManager::GetInstance()->AddCollider(collider);

	// コライダーを更新
	collider->Update();
}