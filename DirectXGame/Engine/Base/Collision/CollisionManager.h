#pragma once
#include <forward_list>

class BaseCollider;

class CollisionManager
{
public:	// 静的メンバ関数
	static CollisionManager* GetInstance();

public:	// メンバ関数
	/// <summary>
	/// コライダーの追加
	/// </summary>
	/// <param name="collider">追加するコライダー</param>
	inline void AddCollider(BaseCollider* collider)
	{
		colliders_.push_front(collider);
	}

	/// <summary>
	/// コライダーの削除
	/// </summary>
	/// <param name="collider">削除するコライダー</param>
	inline void RemoveCollider(BaseCollider* collider)
	{
		colliders_.remove(collider);
	}

	/// <summary>
	/// 全ての衝突をチェック
	/// </summary>
	void CheckAllCollisions();

private:
	// コンストラクタ/デストラクタ
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager&) = delete;

	// コライダーのリスト
	std::forward_list<BaseCollider*> colliders_;
};