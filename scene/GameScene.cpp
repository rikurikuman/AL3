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
	delete spriteA;
	delete spriteB;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	debugCamera = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	textureHandle = TextureManager::Load("mario.jpg");
	model = Model::Create();

	spriteA = Sprite::Create(TextureManager::Load("reticle.png"), {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2}, {1, 1, 1, 1}, {0.5f, 0.5f}, false, false);
	spriteB = Sprite::Create(TextureManager::Load("scope.png"), { 0, 0 }, { 1, 1, 1, 1 }, { 0, 0 }, false, false);

	random_device device;
	uniform_real_distribution<float> posDist(-20, 20);

	for (int z = 0; z < 9; z++) {
		for (int y = 0; y < 9; y++) {
			for (int x = 0; x < 9; x++) {
				WorldTransform t;
				t.translation_ = { -16.0f + 4.0f * x, -16.0f + 4.0f * y, 35 + 4.0f * z };
				t.Initialize();
				t.UpdateMatrix();
				worldTransforms.emplace_back(t);
			}
		}
	}

	viewProjection.eye = { 0, 0, -25 };
	viewProjection.Initialize();

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection);

	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection);
}

void GameScene::Update() {
	debugCamera->Update();

	float moveSpeed = 0.2f;

	if (input_->PushKey(DIK_A)) {
		viewProjection.target.x -= moveSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		viewProjection.target.x += moveSpeed;
	}
	if (input_->PushKey(DIK_W)) {
		viewProjection.target.y += moveSpeed;
	}
	if (input_->PushKey(DIK_S)) {
		viewProjection.target.y -= moveSpeed;
	}

	bool check = ADS;
	if (input_->TriggerKey(DIK_SPACE)) {
		ADS = !ADS;
		larpStart = viewProjection.fovAngleY;
		larpTime = 0;
	}

	if (input_->TriggerKey(DIK_UPARROW) && scopeMultiplier == 4.0f) {
		scopeMultiplier = 8.0f;
		larpStart = viewProjection.fovAngleY;
		larpTime = 0;	
	}
	if (input_->TriggerKey(DIK_DOWNARROW) && scopeMultiplier == 8.0f) {
		scopeMultiplier = 4.0f;
		larpStart = viewProjection.fovAngleY;
		larpTime = 0;
	}

	larpTime++;
	float t = min(30, larpTime) / 30.0f;

	if (ADS) {
		viewProjection.fovAngleY = (1.0 - t) * larpStart + t * (45 / scopeMultiplier * (3.141592653589793238462643383279f / 180));
	}
	else {
		viewProjection.fovAngleY = (1.0 - t) * larpStart + t * (45 * (3.141592653589793238462643383279f / 180));
	}

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
	for (WorldTransform& worldTransform : worldTransforms) {
		model->Draw(worldTransform, viewProjection, textureHandle);
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
	
	if (ADS) {
		spriteA->Draw();
		spriteB->Draw();
	}

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
