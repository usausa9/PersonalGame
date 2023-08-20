#include "Enemy.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"

bool Enemy::sIsPowerUp_ = false;

// ������
void Enemy::Initialize(std::vector<Vector3>& points, uint8_t enemyKind)
{
	if (enemyKind == uint8_t(EnemyKinds::NORMAL))
	{
		// ���@���f���ǂݍ���
		enemyModel_ = OBJModel::LoadFromOBJ("Cube");
	}
	else if (enemyKind == uint8_t(EnemyKinds::POWER))
	{
		// ���@���f���ǂݍ���
		enemyModel_ = OBJModel::LoadFromOBJ("ICO");
	}

	// �G�̎�ގw��
	enemyKind_ = enemyKind;

	// ���@�̍s�񏉊���
	rotation_ = { 0, 0, 0 };
	position_ = { 0, 0, 0 };
	InitializeObject3D();

	// ���@���f���Ǝ��@�I�u�W�F�N�g��R�Â�
	objModel_ = &enemyModel_;

	// �O������_�̐ݒ�
	trajectory_.SetPositions(points);

	// �R���C�_�[�̒ǉ�
	float radius = 1.3f;
	// ���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(Vector3({ 0, radius, 0 }), radius));
	collider_->SetAttribute(COLLISION_ATTR_ENEMYS);
}

// �X�V
void Enemy::Update(const Matrix4& cameraMatrix)
{
	// �����\������ꎞ�I��false��
	sIsPowerUp_ = false;

	// �G�̋O���X�V
	trajectory_.Update();

	// ���W���J�������
	Vector3 cameraPos = { cameraMatrix.m[3][0],cameraMatrix.m[3][1], cameraMatrix.m[3][2] };
	position_ = Matrix4::Transform(trajectory_.GetNowPosition(), cameraMatrix) + cameraPos;

	// �s��X�V �K���Ăяo��
	UpdateObject3D();
}

// �`��
void Enemy::Draw()
{
	// �I�u�W�F�`��
	DrawObject3D();

	if (isAlive_ != true)
	{
		return;
	}
}

void Enemy::Spawn()
{
	// �G����
	trajectory_.MoveStart(600, true);
}

void Enemy::OnCollision(const CollisionInfo& info)
{
	if (enemyKind_ == uint8_t(EnemyKinds::POWER))
	{
		// �����true��
		sIsPowerUp_ = true;
	}

	// �Փ˔���ɂ�����
	isAlive_ = false;
}