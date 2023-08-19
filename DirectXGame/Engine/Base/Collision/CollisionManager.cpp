#include "CollisionManager.h"

#include "BaseCollider.h"
#include "Collision.h"

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;

	return &instance;
}

void CollisionManager::CheckAllCollisions()
{
	std::forward_list<BaseCollider*>::iterator itA;
	std::forward_list<BaseCollider*>::iterator itB;

	// ���ׂĂ̑g�ݍ��킹�ɂ��đ�������`�F�b�N
	itA = colliders_.begin();
	for (; itA != colliders_.end(); ++itA) 
	{
		itB = itA;
		++itB;

		for (; itB != colliders_.end(); ++itB) 
		{
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

			// �����������Ȃ�X�L�b�v
			if (colA->attribute_ == colB->attribute_) 
			{
				continue;
			}

			// 0�Ƃ��ɋ�
			if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
				colB->GetShapeType() == COLLISIONSHAPE_SPHERE) 
				{
				Sphere* sphereA = dynamic_cast<Sphere*>(colA);
				Sphere* sphereB = dynamic_cast<Sphere*>(colB);
				Vector3 inter;

				if (Collision::Col_SphereToSphere(*sphereA, *sphereB, nullptr, &inter)) 
				{
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
		}
	}
}
