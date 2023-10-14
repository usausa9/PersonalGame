#pragma once

/**
 * @file	Vector3.cpp
 * @brief	自作のVector3つ分関連の処理
**/

class Vector3
{
public:
	// メンバ変数
	float x;	// x成分
	float y;	// y成分
	float z;	// z成分

public:
	// コンストラクタ
	Vector3();							// 零ベクトルとして生成
	Vector3(float x, float y, float z);	// x,y成分を指定しての生成

	// メンバ関数
	float Length() const;					// ノルム(長さ)を求める
	Vector3& Normalize();					// 正規化
	float Dot(const Vector3& v) const;		// 内積を求める
	Vector3 Cross(const Vector3& v) const;	// 外積を求める

	// 単項演算子オーバーロード
	Vector3 operator+() const;
	Vector3 operator-() const;
	Vector3 operator*(const Vector3& v);
	Vector3 operator/(const Vector3& v);

	// 代入演算子オーバーロード
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(const Vector3& v);
	Vector3& operator/=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);

	Vector3& operator=(float num);
};

// 2項演算子オーバーロード
// ※いろんな引数のパターンに対応するため、以下のように準備している
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);