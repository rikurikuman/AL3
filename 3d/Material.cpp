#include "DirectXCommon.h"
#include "Material.h"
#include "TextureManager.h"
#include <DirectXTex.h>
#include <cassert>

using namespace DirectX;
using namespace std;

Material* Material::Create() {
	Material* instance = new Material;

	instance->Initialize();

	return instance;
}

void Material::Initialize() {
	// 定数バッファの生成
	CreateConstantBuffer();
}

void Material::CreateConstantBuffer() {
	HRESULT result;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
	  CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
	  &heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
	  IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

	// 定数バッファとのデータリンク
	result = constBuff_->Map(0, nullptr, (void**)&constMap_);
	assert(SUCCEEDED(result));
}

void Material::LoadTexture(const std::string& directoryPath) {
	// テクスチャなし
	if (textureFilename_.size() == 0) {
		textureFilename_ = "white1x1.png";
	}

	HRESULT result = S_FALSE;

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// ファイルパスを結合
	string filepath = directoryPath + textureFilename_;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load(filepath);
}

void Material::Update() {
	// 定数バッファへデータ転送
	constMap_->ambient = ambient_;
	constMap_->diffuse = diffuse_;
	constMap_->specular = specular_;
	constMap_->alpha = alpha_;
}

void Material::SetGraphicsCommand(
  ID3D12GraphicsCommandList* commandList, UINT rooParameterIndexMaterial,
  UINT rooParameterIndexTexture) {

	// SRVをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
	  commandList, rooParameterIndexTexture, textureHandle_);

	// マテリアルの定数バッファをセット
	commandList->SetGraphicsRootConstantBufferView(
	  rooParameterIndexMaterial, constBuff_->GetGPUVirtualAddress());
}

void Material::SetGraphicsCommand(
  ID3D12GraphicsCommandList* commandList, UINT rooParameterIndexMaterial,
  UINT rooParameterIndexTexture, uint32_t textureHandle) {

	// SRVをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
	  commandList, rooParameterIndexTexture, textureHandle);

	// マテリアルの定数バッファをセット
	commandList->SetGraphicsRootConstantBufferView(
	  rooParameterIndexMaterial, constBuff_->GetGPUVirtualAddress());
}