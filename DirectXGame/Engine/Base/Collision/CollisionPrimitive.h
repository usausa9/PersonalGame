#pragma once

#include "UsaMath.h"

/// <summary>
/// ��
/// </summary>
struct Sphere 
{
	// ���W
	Vector3 position = { 0,0,0 };

	// ���a
	float radius = 1.0f;
};

/// <summary>
/// ����
/// </summary>
struct Plane 
{
	// �@���x�N�g��
	Vector3 normal = { 0,1,0 };

	// ���_(0,0)����̋���
	float distance = 0.0f;
};

/// <summary>
/// �@���t���O�p�`(���v��肪�\��)
/// </summary>
class Triangle 
{
public:
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;

	// �@��
	Vector3 normal;

	/// <summary>
	/// �@���̌v�Z
	/// </summary>
	void ComputeNormal();
};

/// <summary>
/// ���C(������)
/// </summary>
struct Ray 
{
	// �n�_���W
	Vector3 start{ 0,0,0 };

	// ����
	Vector3 dir{ 1,0,0 };
};

/// <summary>
/// �~(2D)
/// </summary>
struct Circle 
{
	// ���S���W
	Vector2 pos{ 0,0 };

	// ���a
	float radius = 0.0f;
};