#pragma once
#include "Input.h"
#include "ViewProjection.h"

/// <summary>
/// デバッグ用カメラ
/// </summary>
class DebugCamera {
	using XMMATRIX = DirectX::XMMATRIX;

  public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	DebugCamera(int window_width, int window_height);

	// 更新
	void Update();

	void SetDistance(float distance) { distance_ = distance; }

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

  private:
	// 入力クラスのポインタ
	Input* input_;
	// カメラ注視点までの距離
	float distance_ = 50;
	// スケーリング
	float scaleX_ = 1.0f;
	float scaleY_ = 1.0f;
	// 回転行列
	XMMATRIX matRot = DirectX::XMMatrixIdentity();
	// ビュープロジェクション
	ViewProjection viewProjection_;

	void MultiplyMatrix(const DirectX::XMMATRIX& matrix);
};
