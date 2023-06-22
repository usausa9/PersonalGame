#pragma once

class Vector4
{
public:
	// �����o�ϐ�
	float x;	// x����
	float y;	// y����
	float z;	// z����
	float w;	// w����

public:
	// �R���X�g���N�^
	Vector4();										// ��x�N�g���Ƃ���
	Vector4(float x, float y, float z, float w);	// �e�������w�肵�Đ���

	float Length()const;
	Vector4& Normalize();
	float Dot(const Vector4& v);

	// �P�����Z�q�I�[�o�[���[�h
	Vector4 operator+() const;
	Vector4 operator-() const;

	// ������Z�q�I�[�o�[���[�h
	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(float s);
	Vector4& operator/=(float s);
};

// 2�����Z�q�I�[�o�[���[�h
// �������Ȉ����̃p�^�[���ɑΉ����邽�߁A�ȉ��̂悤�ɏ������Ă���
const Vector4 operator+(const Vector4& v1, const Vector4& v2);
const Vector4 operator-(const Vector4& v1, const Vector4& v2);
const Vector4 operator*(const Vector4& v, float s);
const Vector4 operator*(float s, const Vector4& v);
const Vector4 operator/(const Vector4& v, float s);