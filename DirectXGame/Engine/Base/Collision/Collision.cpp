#include "Collision.h"

#include <algorithm>

// "球" と "球" の 当たり判定
bool Collision::Col_SphereToSphere(const Sphere& s1, const Sphere& s2, float* distance, Vector3* inter)
{
	Vector3 p1, p2;
	float r1, r2;

	p1 = s1.position;
	p2 = s2.position;
	r1 = s1.radius;
	r2 = s2.radius;

	float dis = (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y) + (p2.z - p1.z) * (p2.z - p1.z);
	float rr = (r1 + r2) * (r1 + r2);

	if (distance) 
	{
		*distance = dis;
	}

	if (dis > rr) 
	{
		return false;
	}

	if (inter) 
	{
		*inter = (p1 + p2) / 2.0f;
	}

	return true;
}

// "球" と "平面" の 当たり判定
bool Collision::Col_SphereToPlane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{
	// 座標系の頂点から球の中心座標への距離
	float distV = sphere.position.Dot(plane.normal);

	// 平面の原点距離を減算することで、平面と球の中心との距離が出る
	float dist = distV - plane.distance;

	// 距離の絶対値が半径より大きければあたっていない
	if (fabsf(dist) > sphere.radius) return false;

	// 疑似交点を算出
	if (inter) 
	{
		// 平面上の最近接点を、疑似交点とする
		*inter = -dist * plane.normal + sphere.position;
	}

	return true;
}

// "点" と "三角形" の 最近接点を求める
void Collision::ClosestPtPointToTriangle(const Vector3& point, const Triangle& triangle, Vector3* closest)
{
	// point が p0 の外側の頂点領域の中にあるかどうかをチェック
	Vector3 p0_p1 = triangle.p1_ - triangle.p0_;
	Vector3 p0_p2 = triangle.p2_ - triangle.p0_;
	Vector3 p0_pt = point - triangle.p0_;

	float d1 = p0_p1.Dot(p0_pt);
	float d2 = p0_p2.Dot(p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f) 
	{
		*closest = triangle.p0_;
		return;
	}

	// point が p1 の外側の頂点領域にあるかどうかをチェック
	Vector3 p1_pt = point - triangle.p1_;

	float d3 = p0_p1.Dot(p1_pt);
	float d4 = p0_p2.Dot(p1_pt);

	if (d3 >= 0.0f && d4 <= d3) 
	{
		*closest = triangle.p1_;
		return;
	}

	// point が p0_p1 の辺領域の中にあるかどうかをチェックし、あれば point の p0_p1 上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) 
	{
		float v = d1 / (d1 - d3);
		*closest = triangle.p0_ + v * p0_p1;
		return;
	}

	// point が p2 の外側の頂点領域の中にいるかどうかをチェック
	Vector3 p2_pt = point - triangle.p2_;

	float d5 = p0_p1.Dot(p2_pt);
	float d6 = p0_p2.Dot(p2_pt);

	if (d6 >= 0.0f && d5 <= d6) 
	{
		*closest = triangle.p2_;
		return;
	}

	// point が p0_p2 の辺領域の中にあるかどうかをチェックし、あれば point の p0_p2 上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) 
	{
		float w = d2 / (d2 - d6);
		*closest = triangle.p0_ + w * p0_p2;
		return;
	}

	// point が p1_p2 の辺領域の中にあるかどうかをチェックし、あれば point の p1_p2 上に対する射影を返す
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) 
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.p1_ + w * (triangle.p2_ - triangle.p1_);
		return;
	}

	float denomination = 1.0f / (va + vb + vc);
	float v = vb * denomination;
	float w = vc * denomination;
	*closest = triangle.p0_ + p0_p1 * v + p0_p2 * w;
}

// "球" と "三角形" の 当たり判定
bool Collision::Col_SphereToTriangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter)
{
	Vector3 p;
	// 球の中心に対する最近接点である三角形上にある点pを見つける
	ClosestPtPointToTriangle(sphere.position, triangle, &p);

	// 点p と 球の中心 の差分ベクトル
	Vector3 v = p - sphere.position;

	// 距離を求める
	// 同じベクトル同士の内積は三平方の定理のルート内部の式と一致する
	float len = v.Dot(v);

	// 球と三角形の距離が半径以下ならあたっていない
	if (len > sphere.radius * sphere.radius) return false;
	// 疑似交点を計算
	if (inter) 
	{
		// 三角形上の最近接点pを疑似交点とする
		*inter = p;
	}

	return true;
}

// "レイ" と "平面" の 当たり判定
bool Collision::Col_RayToPlane(const Ray& ray, const Plane& plane, float* distance, Vector3* inter)
{
	const float epsilon = 1.0e-5f;	// 誤差吸収用の微小な値

	// 面法線とレイの方向ベクトルの内積
	float d1 = plane.normal.Dot(ray.dir);

	// 裏面には当たらない
	if (d1 > -epsilon) { return false; }

	// 始点と原点の距離(平面の法線方向)
	// 面法線とレイの始点座標(位置ベクトル)の内積
	float d2 = plane.normal.Dot(ray.start);
	// 始点と平面の距離(平面の法線方向)
	float dist = d2 - plane.distance;
	// 始点と平面の距離(レイ方向)
	float t = dist / -d1;

	// 交点が始点より後ろにあるので、当たらない
	if (t < 0) return false;

	// 距離を書き込む
	if (distance) { *distance = t; }

	// 交点を計算
	if (inter) { *inter = ray.start + t * ray.dir; }

	return true;
}

// "レイ" と "三角形" の 当たり判定
bool Collision::Col_RayToTriangle(const Ray& ray, const Triangle& triangle, float* distance, Vector3* inter)
{
	// 三角形が乗っている平面を算出
	Plane plane;
	Vector3 interPlane;
	plane.normal = triangle.normal_;
	plane.distance = triangle.normal_.Dot(triangle.p0_);

	// レイと平面があたっていなければ、当たっていない
	if (!Col_RayToPlane(ray, plane, distance, &interPlane)) { return false; }

	// レイと平面が当たっているので、距離と交点が書き込まれた
	// レイと平面の交点が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f;	// 誤差吸収用の微小な値
	Vector3 m;

	// 辺p0_p1について
	Vector3 pt_p0 = triangle.p0_ - interPlane;
	Vector3 p0_p1 = triangle.p1_ - triangle.p0_;
	m = pt_p0.Cross(p0_p1);
	// 辺の外側であれば当たっていないので判定を打ち切る
	if (m.Dot(triangle.normal_) < -epsilon) { return false; }

	// 辺p1_p2について
	Vector3 pt_p1 = triangle.p1_ - interPlane;
	Vector3 p1_p2 = triangle.p2_ - triangle.p1_;
	m = pt_p1.Cross(p1_p2);
	// 辺の外側であれば当たっていないので判定を打ち切る
	if (m.Dot(triangle.normal_) < -epsilon) { return false; }

	// 辺p2_p0について
	Vector3 pt_p2 = triangle.p2_ - interPlane;
	Vector3 p2_p0 = triangle.p0_ - triangle.p2_;
	m = pt_p2.Cross(p2_p0);
	// 辺の外側であれば当たっていないので判定を打ち切る
	if (m.Dot(triangle.normal_) < -epsilon) { return false; }

	// 内側の場合、当たっている
	if (inter) 
	{
		*inter = interPlane;
	}

	return true;
}

// "レイ" と "球" の 当たり判定
bool Collision::Col_RayToSphere(const Ray& ray, const Sphere& sphere, float* distance, Vector3* inter)
{
	Vector3 m = ray.start - sphere.position;
	float b = m.Dot(ray.dir);
	float c = m.Dot(m) - sphere.radius * sphere.radius;
	// rayの始点がsphereの外側にあり(c>0)、rayがsphereから離れていく方向を指している場合(b>0)当たらない
	if (c > 0.0f && b > 0.0f) { return false; }

	float discr = b * b - c;
	// 負の判別式はレイが球を離れていることに一致
	if (discr < 0.0f) { return false; }

	// レイは球と交差している
	// 交差する最小の値tを計算
	float t = -b - sqrtf(discr);
	// tが負である場合、レイは球の内側から開始しているのでtを0にクランプ
	t = std::max( t, 0.0f );

	if (distance) 
	{ 
		*distance = t; 
	}

	if (inter) 
	{ 
		*inter = ray.start + t * ray.dir;
	}

	return true;
}

// "円" と "円" の 当たり判定
bool Collision::Col_CircleToCircle(const Circle& c1, const Circle& c2)
{
	float dis = (c2.pos.x - c1.pos.x) * (c2.pos.x - c1.pos.x) + (c2.pos.y - c1.pos.y) * (c2.pos.y - c1.pos.y);
	float rr = (c1.radius + c2.radius) * (c1.radius + c2.radius);

	if (dis > rr) 
	{
		return false;
	}

	return true;
}