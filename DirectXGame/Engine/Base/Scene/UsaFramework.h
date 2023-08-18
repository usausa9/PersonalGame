#pragma once
#include "Common.h"

#include "IScene.h"
#include "SceneManager.h"

#include "Input.h"
#include "WinAPI.h"

#include "SpriteManager.h"
#include "ImGuiManager.h"

#include "AbstractSceneFactory.h"

class UsaFramework
{
public:
	SceneManager* sceneManager_ = nullptr;

public:
	// 初期化
	virtual void Initialize();

	// 終了
	virtual void Finalize();

	// 更新処理
	virtual void Update();

	// 更新後処理
	void PostUpdate();

	// 描画前処理
	void PreDraw();

	// パーティクル描画前処理
	void PreDrawParticle();

	// 描画
	// 純粋仮想関数にすることでオーバーライドを必須に
	virtual void Draw(); 

	// 描画後処理
	void PostDraw();

	// 終了チェック
	virtual bool IsEndRequest() { return endRequest_; }

	// 仮想デストラクタ
	virtual ~UsaFramework() = default;

public:
	// 実行
	void Run();

protected: // メンバ変数
	// シーン工場
	AbstractSceneFactory* sceneFactory_ = nullptr;

	// スプライトマネージャ
	unique_ptr<SpriteManager> spriteManager_ = nullptr;

	// 終了フラグ
	bool endRequest_ = false;
	// パイプラインステート
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

	// WindowsAPI初期化
	WinAPI& window_ = *WinAPI::GetInstance();
};

