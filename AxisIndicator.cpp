#include "AxisIndicator.h"
#include <cassert>

const float AxisIndicator::kViewPortWidth = 100;
const float AxisIndicator::kViewPortHeight = 100;
const float AxisIndicator::kViewPortTopLeftX = 1280 - kViewPortWidth;
const float AxisIndicator::kViewPortTopLeftY = 0;
const float AxisIndicator::kCameraDistance = 10.0f;
const std::string AxisIndicator::kModelName = "axis";

AxisIndicator* AxisIndicator::GetInstance() {
	static AxisIndicator instance;
	return &instance;
}

void AxisIndicator::SetTargetViewProjection(const ViewProjection* targetViewProjection) {
	AxisIndicator* instance = GetInstance();

	instance->targetViewProjection_ = targetViewProjection;
}

void AxisIndicator::SetVisible(bool isVisible) {

	AxisIndicator* instance = GetInstance();

	instance->isVisible_ = isVisible;
}

void AxisIndicator::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.eye = {0, 0, kCameraDistance};
	viewProjection_.aspectRatio = (float)kViewPortWidth / kViewPortHeight;
	viewProjection_.Initialize();
	// モデルの生成
	model_.reset(Model::CreateFromOBJ(kModelName, false));
}

void AxisIndicator::Update() {

	if (targetViewProjection_) {

		DirectX::XMVECTOR eye = {0, 0, -kCameraDistance, 0};
		DirectX::XMVECTOR up = {0, 1, 0, 0};
		// 平行移動成分をクリアしてから転置することでビューの回転の逆行列を得る
		DirectX::XMMATRIX reverse = targetViewProjection_->matView;
		reverse.r[3] = {0, 0, 0, 1};
		reverse = DirectX::XMMatrixTranspose(reverse);
		// 原点から視線方向の逆に進んだ位置にカメラを配置
		eye = DirectX::XMVector3TransformNormal(eye, reverse);
		// 上方向を合わせる
		up = DirectX::XMVector3TransformNormal(up, reverse);
		// 軸方向表示用のビュープロジェクション行列を計算
		DirectX::XMStoreFloat3(&viewProjection_.eye, eye);
		DirectX::XMStoreFloat3(&viewProjection_.up, up);
		viewProjection_.UpdateMatrix();
	}
}

void AxisIndicator::Draw() {

	if (!isVisible_) {
		return;
	}

	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// ビューポートの設定
	CD3DX12_VIEWPORT viewport =
	  CD3DX12_VIEWPORT(kViewPortTopLeftX, kViewPortTopLeftY, kViewPortWidth, kViewPortHeight);
	commandList->RSSetViewports(1, &viewport);
	// シザリング矩形の設定
	CD3DX12_RECT rect = CD3DX12_RECT(
	  (LONG)kViewPortTopLeftX, (LONG)kViewPortTopLeftY,
	  static_cast<LONG>(kViewPortTopLeftX + kViewPortWidth),
	  static_cast<LONG>(kViewPortTopLeftY + kViewPortHeight));
	commandList->RSSetScissorRects(1, &rect);

	// モデル描画
	Model::PreDraw(commandList);
	model_->Draw(worldTransform_, viewProjection_);
	Model::PostDraw();
}
