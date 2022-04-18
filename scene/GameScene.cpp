#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;
using namespace std;

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	transform.Initialize();

	viewProjection_.eye.y = 20;
	viewProjection_.Initialize();
	textureHandle_ = TextureManager::Load("mario.jpg");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();
}

void GameScene::Update() {
	if (input_->PushKey(DIK_A)) {
		transform.rotation_.y -= 0.01f;
	}
	if (input_->PushKey(DIK_D)) {
		transform.rotation_.y += 0.01f;
	}

	transform.UpdateMatrix();

	XMFLOAT3 front(0, 0, 1);
	XMFLOAT3 result(0, 0, 1);
	//y軸回りの回転行列演算
	result.x = cos(transform.rotation_.y) * front.x + sin(transform.rotation_.y) * front.z;
	result.z = (-sin(transform.rotation_.y)) * front.x + cos(transform.rotation_.y) * front.z;

	float moveSpeed = 0.02f;
	if (input_->PushKey(DIK_W)) {
		transform.translation_.x += moveSpeed * result.x;
		transform.translation_.z += moveSpeed * result.z;
	}
	if (input_->PushKey(DIK_S)) {
		transform.translation_.x -= moveSpeed * result.x;
		transform.translation_.z -= moveSpeed * result.z;
	}

	transform.UpdateMatrix();

	debugText_->Print("objPos:(" + to_string(transform.translation_.x) + ", " + to_string(transform.translation_.y) + ", " + to_string(transform.translation_.z) + ")", 20, 20, 1);
	debugText_->Print("objRotR:(" + to_string(transform.rotation_.x) + ", " + to_string(transform.rotation_.y) + ", " + to_string(transform.rotation_.z) + ")", 20, 40, 1);
	debugText_->Print("objRotD:(" + to_string(XMConvertToDegrees(transform.rotation_.x)) + ", " + to_string(XMConvertToDegrees(transform.rotation_.y)) + ", " + to_string(XMConvertToDegrees(transform.rotation_.z)) + ")", 20, 60, 1);
	debugText_->Print("frontRotR:(" + to_string(result.x) + ", " + to_string(result.y) + ", " + to_string(result.z) + ")", 20, 80, 1);
	debugText_->Print("frontRotD:(" + to_string(XMConvertToDegrees(result.x)) + ", " + to_string(XMConvertToDegrees(result.y)) + ", " + to_string(XMConvertToDegrees(result.z)) + ")", 20, 100, 1);

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	model_->Draw(transform, viewProjection_, textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
