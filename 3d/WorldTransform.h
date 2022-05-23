#pragma once

#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>

// 定数バッファ用データ構造体
struct ConstBufferDataWorldTransform {
	DirectX::XMMATRIX matWorld; // ローカル → ワールド変換行列
};

/// <summary>
/// ワールド変換データ
/// </summary>
struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataWorldTransform* constMap = nullptr;
	// ローカルスケール
	DirectX::XMFLOAT3 scale_ = {1, 1, 1};
	// X,Y,Z軸回りのローカル回転角
	DirectX::XMFLOAT3 rotation_ = {0, 0, 0};
	// ローカル座標
	DirectX::XMFLOAT3 translation_ = {0, 0, 0};
	// ローカル → ワールド変換行列
	DirectX::XMMATRIX matWorld_;
	// 親となるワールド変換へのポインタ
	WorldTransform* parent_ = nullptr;

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
