#pragma once

/**
 * @file	Vector2.h
 * @brief	自作のVector2つ分関連の処理
**/

class Vector2
{
public:
	// メンバ変数
	float x; // x成分
	float y; // y成分

public:
	// コンストラクタ
	Vector2();							// 零ベクトルとして生成
	Vector2(float x, float y);			// x,y成分を指定しての生成

	// メンバ関数
	float length() const;				// ノルム(長さ)を求める
	Vector2& normalize();				// 正規化
	float dot(const Vector2& v) const;	// 内積を求める
	float cross(const Vector2& v) const;// 外積を求める

	// 単項演算子オーバーロード
	Vector2 operator+() const;
	Vector2 operator-() const;

	// 代入演算子オーバーロード
	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(float s);
	Vector2& operator/=(float s);
};

// 2項演算子オーバーロード
// ※いろんな引数のパターンに対応するため、以下のように準備している
const Vector2 operator+(const Vector2& v1, const Vector2& v2);
const Vector2 operator-(const Vector2& v1, const Vector2& v2);
const Vector2 operator*(const Vector2& v, float s);
const Vector2 operator*(float s, const Vector2& v);
const Vector2 operator/(const Vector2& v, float s);