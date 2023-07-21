#pragma once

#include "UsaMath.h"

/// �s��
class Matrix4 
{
public:
	// �s��
	float m[4][4];

	// �P�ʍs����
	static Matrix4 Identity();
	Matrix4 Identity(Matrix4 matrix);

	// �s��̃X�J���[
	Matrix4 ScalarProduct(Matrix4 matrix, Vector3 scalar);
	
	// �s�񓯎m�̊|���Z
	Matrix4 Matrix4Product(Matrix4 matrix, Matrix4 matrix2);
	
	// �g��k���s��
	Matrix4 Scale(const Vector3& s);
	
	// ��]�s��
	Matrix4 RotateX(float angle);
	Matrix4 RotateY(float angle);
	Matrix4 RotateZ(float angle);
	
	// ���s�ړ��s��쐬
	Matrix4 Translate(const Vector3& t);

	// ���W�ϊ��i�x�N�g���ƍs��̊|���Z�j
	static Vector3 Transform(const Vector3& v, const Matrix4& m);
	static Vector3 TransformDivW(const Vector3& v, const Matrix4& m);

	// �t�s�񐶐�
	static Matrix4 Inverse(const Matrix4 m);

	// �r���[�s��֘A
	Matrix4 CreateViewMat(const Vector3& eye, const Vector3& target, const Vector3& up);
	Matrix4 CreateProjectionMat(float fovY, float aspectRatio, float nearZ, float farZ);

	// ������Z�q�I�[�o�[���[�h
	Matrix4& operator*=(const Matrix4& m2);
};

// 2�����Z�q�I�[�o�[���[�h
const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
const Vector3 operator*(const Vector3& v, const Matrix4& m);