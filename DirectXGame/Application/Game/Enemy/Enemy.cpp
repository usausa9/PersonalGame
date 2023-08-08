#include "Enemy.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"

// ������
void Enemy::Initialize(std::vector<Vector3>& points)
{
	// ���@���f���ǂݍ���
	enemyModel = OBJModel::LoadFromOBJ("Cube");

	// ���@�̍s�񏉊���
	rotation = { 0, 0, 0 };
	position = { 0, 0, 0 };
	InitializeObject3D();

	// ���@���f���Ǝ��@�I�u�W�F�N�g��R�Â�
	objModel = &enemyModel;

	// �O������_�̐ݒ�
	trajectory.SetPositions(points);

	// �R���C�_�[�̒ǉ�
	float radius = 1.3f;
	// ���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(Vector3({ 0, radius, 0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
}

// �X�V
void Enemy::Update(const Matrix4& cameraMatrix, const Vector3& playerWorldPosition)
{
	// �G�̋O���X�V
	trajectory.Update();

	// ���W���J�������
	Vector3 cameraPos = { cameraMatrix.m[3][0],cameraMatrix.m[3][1], cameraMatrix.m[3][2] };
	position = Matrix4::Transform(trajectory.GetNowPosition(), cameraMatrix) + cameraPos;

	// �ړ�����
	Move();

	// �s��X�V �K���Ăяo��
	UpdateObject3D();
}

// �`��
void Enemy::Draw()
{
	// �I�u�W�F�`��
	DrawObject3D();

	if (isAlive != true)
	{
		return;
	}
}

void Enemy::Spawn()
{
	// �G����
	trajectory.MoveStart(600, true);
}

void Enemy::OnCollision(const CollisionInfo& info)
{
	// �Փ˔���ɂ�����
	//isAlive = false;
}

// ���͎󂯕t�� + �ړ�
void Enemy::Move()
{
	// �ړ��ʂ̏�����
	move = { 1.5f,0,0 };


	// �ړ��ʂ̉��Z
	position += move;

}