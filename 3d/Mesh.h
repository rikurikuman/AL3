#pragma once

#include "Material.h"
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <unordered_map>
#include <vector>
#include <wrl.h>

/// <summary>
/// 形状データ
/// </summary>
class Mesh {
  private: // エイリアス
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

  public: // サブクラス
	// 頂点データ構造体（テクスチャあり）
	struct VertexPosNormalUv {
		XMFLOAT3 pos;    // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;     // uv座標
	};

  public: // メンバ関数
	/// <summary>
	/// 名前を取得
	/// </summary>
	/// <returns>名前</returns>
	const std::string& GetName() { return name_; }

	/// <summary>
	/// 名前をセット
	/// </summary>
	/// <param name="name">名前</param>
	void SetName(const std::string& name_);

	/// <summary>
	/// 頂点データの追加
	/// </summary>
	/// <param name="vertex">頂点データ</param>
	void AddVertex(const VertexPosNormalUv& vertex);

	/// <summary>
	/// 頂点インデックスの追加
	/// </summary>
	/// <param name="index">インデックス</param>
	void AddIndex(unsigned short index);

	/// <summary>
	/// 頂点データの数を取得
	/// </summary>
	/// <returns>頂点データの数</returns>
	inline size_t GetVertexCount() { return vertices_.size(); }

	/// <summary>
	/// エッジ平滑化データの追加
	/// </summary>
	/// <param name="indexPosition">座標インデックス</param>
	/// <param name="indexVertex">頂点インデックス</param>
	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);

	/// <summary>
	/// 平滑化された頂点法線の計算
	/// </summary>
	void CalculateSmoothedVertexNormals();

	/// <summary>
	/// マテリアルの取得
	/// </summary>
	/// <returns>マテリアル</returns>
	Material* GetMaterial() { return material_; }

	/// <summary>
	/// マテリアルの割り当て
	/// </summary>
	/// <param name="material">マテリアル</param>
	void SetMaterial(Material* material);

	/// <summary>
	/// バッファの生成
	/// </summary>
	void CreateBuffers();

	/// <summary>
	/// 頂点バッファ取得
	/// </summary>
	/// <returns>頂点バッファ</returns>
	const D3D12_VERTEX_BUFFER_VIEW& GetVBView() { return vbView_; }

	/// <summary>
	/// インデックスバッファ取得
	/// </summary>
	/// <returns>インデックスバッファ</returns>
	const D3D12_INDEX_BUFFER_VIEW& GetIBView() { return ibView_; }

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList">命令発行先コマンドリスト</param>
	/// <param name="rooParameterIndexMaterial">マテリアルのルートパラメータ番号</param>
	/// <param name="rooParameterIndexTexture">テクスチャのルートパラメータ番号</param>
	void Draw(
	  ID3D12GraphicsCommandList* commandList, UINT rooParameterIndexMaterial,
	  UINT rooParameterIndexTexture);

	/// <summary>
	/// 描画（テクスチャ差し替え版）
	/// </summary>
	/// <param name="commandList">命令発行先コマンドリスト</param>
	/// <param name="rooParameterIndexMaterial">マテリアルのルートパラメータ番号</param>
	/// <param name="rooParameterIndexTexture">テクスチャのルートパラメータ番号</param>
	/// <param name="textureHandle">差し替えるテクスチャハンドル</param>
	void Draw(
	  ID3D12GraphicsCommandList* commandList, UINT rooParameterIndexMaterial,
	  UINT rooParameterIndexTexture, uint32_t textureHandle);

	/// <summary>
	/// 頂点配列を取得
	/// </summary>
	/// <returns>頂点配列</returns>
	inline const std::vector<VertexPosNormalUv>& GetVertices() { return vertices_; }

	/// <summary>
	/// インデックス配列を取得
	/// </summary>
	/// <returns>インデックス配列</returns>
	inline const std::vector<unsigned short>& GetIndices() { return indices_; }

  private: // メンバ変数
	// 名前
	std::string name_;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_ = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_ = {};
	// 頂点データ配列
	std::vector<VertexPosNormalUv> vertices_;
	// 頂点インデックス配列
	std::vector<unsigned short> indices_;
	// 頂点法線スムージング用データ
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData_;
	// マテリアル
	Material* material_ = nullptr;
};
