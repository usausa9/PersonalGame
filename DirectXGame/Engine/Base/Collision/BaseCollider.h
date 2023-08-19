#pragma once
#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "Object3d.h"

/// <summary>
/// �R���C�_�[���N���X
/// </summary>
class BaseCollider
{
public:
	friend class CollisionManager;

public:
	BaseCollider() = default;
	virtual ~BaseCollider() = default;

	inline void SetObject(Object3D* object) 
	{
		this->object3d_ = object;
	}

	inline Object3D* GetObject3d()
	{
		return object3d_;
	}

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update() = 0;

	inline CollisionShapeType GetShapeType() { return shapeType_; }

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	inline void OnCollision(const CollisionInfo& info) 
	{
		object3d_->OnCollision(info);
	}

	inline void SetAttribute(unsigned short attribute) { this->attribute_ = attribute; }
	inline void AddAttribute(unsigned short attribute) { this->attribute_ |= attribute; }
	inline void RemoveAttribute(unsigned short attribute) { this->attribute_ &= !attribute; }

protected:
	Object3D* object3d_ = nullptr;
	// �`��^�C�v
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

	// �����蔻�葮��
	unsigned short attribute_ = 0b1111111111111111;
};

