//#pragma once
//
//class UsaFramework
//{
//public:
//	// 初期化
//	virtual void Initialize();
//
//	// 終了
//	virtual void Finalize();
//
//	// 更新処理
//	virtual void Update();
//
//	// 描画
//	// 純粋仮想関数にすることでオーバーライドを必須に
//	virtual void Draw() = 0; 
//
//	// 終了チェック
//	virtual bool IsEndRequest() { return endRequest_; }
//
//	// 仮想デストラクタ
//	virtual ~UsaFramework() = default;
//
//private: // メンバ変数
//	// 終了フラグ
//	bool endRequest_ = false;
//};
//
