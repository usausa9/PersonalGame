#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include "Vector3.h"

/// <summary>
/// ���Փ˔���I�u�W�F�N�g
/// </summary>
class SphereCollider : public BaseCollider, public Sphere
{
public:
	SphereCollider(Vector3 offset = { 0,0,0 }, float radius = 1.0f) :
		offset_(offset),
		radius_(radius)
	{
		// ���`����Z�b�g
		shapeType_ = COLLISIONSHAPE_SPHERE;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	inline void SetRadius(float radius) { this->radius_ = radius; }

private:
	// �I�u�W�F�N�g���S����̃I�t�Z�b�g
	Vector3 offset_;
	// ���a
	float radius_;
};