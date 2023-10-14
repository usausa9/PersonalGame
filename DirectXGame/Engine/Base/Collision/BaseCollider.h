#pragma once

/**
 * @file	BaseCollider.h
 * @brief	コライダーの基底処理
**/

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "Object3d.h"

class BaseCollider
{
public:
	friend class CollisionManager;

public:
	// コンストラクタ/デストラクタ
	BaseCollider() = default;
	virtual ~BaseCollider() = default;

	// オブジェのセッター
	inline void SetObject(Object3D* object)
	{
		this->object3d_ = object;
	}

	// オブジェのゲッター
	inline Object3D* GetObject3d()
	{
		return object3d_;
	}

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	// ゲッター
	inline CollisionShapeType GetShapeType() { return shapeType_; }

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	inline void OnCollision(const CollisionInfo& info)
	{
		object3d_->OnCollision(info);
	}

	// アトリビュートのSet,Add,Remove
	inline void SetAttribute(unsigned short attribute) { this->attribute_ = attribute; }
	inline void AddAttribute(unsigned short attribute) { this->attribute_ |= attribute; }
	inline void RemoveAttribute(unsigned short attribute) { this->attribute_ &= !attribute; }

protected:
	Object3D* object3d_ = nullptr;
	// 形状タイプ
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

	// 当たり判定属性
	unsigned short attribute_ = 0b1111111111111111;
};

