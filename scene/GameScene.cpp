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
	uniform_real_distribution<float> posDist(-20, 20);

	WorldTransform t;
	t.Initialize();
	worldTransforms.emplace_back(t);

	for (int i = 0; i < 3; i++) {
		ViewProjection v;
		v.eye = { posDist(device), posDist(device), posDist(device) };
		v.Initialize();
		viewProjections.emplace_back(v);
	}

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjections[cameraIndex]);

	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjections[cameraIndex]);
}

void GameScene::Update() {
	debugCamera->Update();

	Vector3 eyeMove = Vector3();
	Vector3 targetMove = Vector3();
	const float moveSpeed = 0.2f;

	if (input_->TriggerKey(DIK_SPACE)) {
		cameraIndex++;
		if (cameraIndex >= viewProjections.size()) {
			cameraIndex = 0;
		}
	}

	for (int i = 0; i < viewProjections.size(); i++) {
		debugText_->Print("Camera " + to_string(i+1), 50, 50 + 100 * i, 1);
		debugText_->Print("eye:(" + to_string(viewProjections[i].eye.x) + ", " + to_string(viewProjections[i].eye.y) + ", " + to_string(viewProjections[i].eye.z) + ")", 50, 70 + 100 * i, 1);
		debugText_->Print("target:(" + to_string(viewProjections[i].target.x) + ", " + to_string(viewProjections[i].target.y) + ", " + to_string(viewProjections[i].target.z) + ")", 50, 90 + 100 * i, 1);
		debugText_->Print("fovAngleY:(" + to_string(viewProjections[i].fovAngleY * (180 / 3.141592653589793238462643383279f)) + ")", 50, 110 + 100 * i, 1);
	}

	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjections[cameraIndex]);

	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjections[cameraIndex]);
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
	for (WorldTransform& worldTransform : worldTransforms) {
		model->Draw(worldTransform, viewProjections[cameraIndex], textureHandle);
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
