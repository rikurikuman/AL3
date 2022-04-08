#pragma once

#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>

// 定数バッファ用データ構造体
struct ConstBufferDataViewProjection {
	DirectX::XMMATRIX view;       // ワールド → ビュー変換行列
	DirectX::XMMATRIX projection; // ビュー → プロジェクション変換行列
	DirectX::XMFLOAT3 cameraPos;  // カメラ座標（ワールド座標）
};

/// <summary>
/// ビュープロジェクション変換データ
/// </summary>
struct ViewProjection {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataViewProjection* constMap = nullptr;

#pragma region ビュー行列の設定
	// 視点座標
	DirectX::XMFLOAT3 eye = {0, 0, -50.0f};
	// 注視点座標
	DirectX::XMFLOAT3 target = {0, 0, 0};
	// 上方向ベクトル
	DirectX::XMFLOAT3 up = {0, 1, 0};
#pragma endregion

#pragma region 射影行列の設定
	// 垂直方向視野角
	float fovAngleY = DirectX::XMConvertToRadians(45.0f);
	// ビューポートのアスペクト比
	float aspectRatio = (float)16 / 9;
	// 深度限界（手前側）
	float nearZ = 0.1f;
	// 深度限界（奥側）
	float farZ = 1000.0f;
#pragma endregion

	// ビュー行列
	DirectX::XMMATRIX matView;
	// 射影行列
	DirectX::XMMATRIX matProjection;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を更新する
	/// </summary>
	void UpdateMatrix();
};
