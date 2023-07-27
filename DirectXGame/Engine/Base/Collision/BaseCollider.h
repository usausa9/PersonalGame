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
		this->object3d = object;
	}

	inline Object3D* GetObject3d() {

		return object3d;
	}

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update() = 0;

	inline CollisionShapeType GetShapeType() { return shapeType; }

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	inline void OnCollision(const CollisionInfo& info) 
	{
		object3d->OnCollision(info);
	}

	inline void SetAttribute(unsigned short attribute) { this->attribute = attribute; }
	inline void AddAttribute(unsigned short attribute) { this->attribute |= attribute; }
	inline void RemoveAttribute(unsigned short attribute) { this->attribute &= !attribute; }

protected:
	Object3D* object3d = nullptr;
	// �`��^�C�v
	CollisionShapeType shapeType = SHAPE_UNKNOWN;

	// �����蔻�葮��
	unsigned short attribute = 0b1111111111111111;
};

