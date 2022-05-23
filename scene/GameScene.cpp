#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <cassert>
#include <random>

using namespace std;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera;
	delete model;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	debugCamera = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	textureHandle = TextureManager::Load("mario.jpg");
	model = Model::Create();

	random_device device;
	uniform_real_distribution<float> rotDist(0.0f, 1.0f);
	uniform_real_distribution<float> posDist(-20, 20);

	for (int i = 0; i < 100; i++) {
		worldTransforms[i].Initialize();

		worldTransforms[i].matWorld_ = Matrix4::Identity();
		worldTransforms[i].matWorld_ *= Matrix4::RotationZXY(rotDist(device), rotDist(device), rotDist(device));
		worldTransforms[i].matWorld_ *= Matrix4::Translation(posDist(device), posDist(device), posDist(device));
		worldTransforms[i].TransferMatrix();
	}

	viewProjection.eye = { 0, 0, -10 };
	viewProjection.Initialize();

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection);

	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection);
}

void GameScene::Update() {
	debugCamera->Update();

	Vector3 eyeMove = Vector3();
	Vector3 targetMove = Vector3();
	const float moveSpeed = 0.2f;

	if (input_->PushKey(DIK_W)) {
		Vector3 z = (viewProjection.target - viewProjection.eye).Normalize();
		eyeMove += z * moveSpeed;
		targetMove += z * moveSpeed;
	}
	else if (input_->PushKey(DIK_S)) {
		Vector3 z = (viewProjection.target - viewProjection.eye).Normalize();
		eyeMove -= z * moveSpeed;
		targetMove -= z * moveSpeed;
	}

	if (input_->PushKey(DIK_A)) {
		Vector3 x = viewProjection.up.Cross((viewProjection.target - viewProjection.eye));
		x.Normalize();
		targetMove -= x * moveSpeed;
	}
	else if (input_->PushKey(DIK_D)) {
		Vector3 x = viewProjection.up.Cross((viewProjection.target - viewProjection.eye));
		x.Normalize();
		targetMove += x * moveSpeed;
	}

	if (input_->PushKey(DIK_UP)) {
		viewProjection.fovAngleY += 0.01f;
		if (viewProjection.fovAngleY > 3.141592653589793238462643383279f) {
			viewProjection.fovAngleY = 3.141592653589793238462643383279f;
		}
	}
	else if (input_->PushKey(DIK_DOWN)) {
		viewProjection.fovAngleY -= 0.01f;
		if (viewProjection.fovAngleY < 0.01f) {
			viewProjection.fovAngleY = 0.01f;
		}
	}

	viewProjection.eye += eyeMove;
	viewProjection.target += targetMove;
	viewProjection.UpdateMatrix();

	debugText_->Print("eye:(" + to_string(viewProjection.eye.x) + ", " + to_string(viewProjection.eye.y) + ", " + to_string(viewProjection.eye.z) + ")", 50, 50, 1);
	debugText_->Print("target:(" + to_string(viewProjection.target.x) + ", " + to_string(viewProjection.target.y) + ", " + to_string(viewProjection.target.z) + ")", 50, 70, 1);
	debugText_->Print("fovAngleY:(" + to_string(viewProjection.fovAngleY * (180 / 3.141592653589793238462643383279f)) + ")", 50, 90, 1);
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
	for (int i = 0; i < 100; i++) {
		model->Draw(worldTransforms[i], viewProjection, textureHandle);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion


	for (int i = -30; i <= 30; i++) {
		Vector4 color = {0.5f, 0.5f, 0.5f, 1};

		if (i % 5 == 0) {
			color = { 0, 1, 0, 1 };
		}

		PrimitiveDrawer::GetInstance()->DrawLine3d({ (float)i, 0.0f, -30.0f }, { (float)i, 0.0f, 30.0f }, color);
		PrimitiveDrawer::GetInstance()->DrawLine3d({ -30.0f, 0.0f, (float)i }, { 30.0f, 0.0f, (float)i }, color);
	}


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
