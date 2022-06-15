#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include "math.h"
#include <cassert>
#include <random>

using namespace DirectX;

#define PI 3.14159265358979323846264338327950288

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	model_ = Model::Create();

	//--------乱数--------

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());

	//回転角用

	//乱数範囲の指定
	std::uniform_real_distribution<float> rotDist(0.0f, PI * 2);

	//座標用

	//乱数範囲の指定
	std::uniform_real_distribution<float> transDist(-10.0f, 10.0f);

	float rotX;
	float rotY;
	float rotZ;

	float transX;
	float transY;
	float transZ;

	//ワールドトランスフォーム

	for (WorldTransform& worldTransform : worldTransforms_) {

		//ワールドトランスフォームの初期化
		worldTransform.Initialize();

		rotX = rotDist(engine);
		rotY = rotDist(engine);
		rotZ = rotDist(engine);

		transX = transDist(engine);
		transY = transDist(engine);
		transZ = transDist(engine);

		//拡大縮小
		Vector3 scare_ = {1.0f, 1.0f, 1.0f};
		//回転
		Vector3 rot_ = {rotX, rotY, rotZ};
		//平行移動
		Vector3 trans_ = {transX, transY, transZ};

		//単位行列を代入する
		worldTransform.matWorld_ = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		                            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

		//ワールド変換行列
		worldTransform.matWorld_ =
		  World(Scale(scare_), Rot(RotX(rot_.x), RotY(rot_.y), RotZ(rot_.z)), Trans(trans_));

		//行列の転送
		worldTransform.TransferMatrix();
	}

	////	カメラ視点座標を設定
	// viewProjection_.eye = {0,0,-10};

	////カメラ注視点座標を設定
	// viewProjection_.target = {10,0,0};

	////カメラ上方向ベクトルを設定(右上45度指定)
	// viewProjection_.up = {cosf(PI / 4.0f), sinf(PI / 4.0f),0.0f};

	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = (PI / 180) * (10.0f);

	//アスペクト比を設定
	viewProjection_.aspectRatio = 1.0f;

	//ニアクリップを設定
	viewProjection_.nearZ = -1.0f;

	//ファークリップ距離を設定
	viewProjection_.farZ = 53.0f;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバックカメラの生成
	debugCamera_ = new DebugCamera(500, 500);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {

	//デバックカメラの更新
	debugCamera_->Update();

	////視点移動処理
	//{
	//	//視点の移動ベクトル
	//	Vector3 move = {0, 0, 0};

	//	//視点移動速さ
	//	const float kEyeSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_W)) {
	//		move.z += kEyeSpeed;
	//	} else if (input_->PushKey(DIK_S)) {
	//		move.z -= kEyeSpeed;
	//	}

	//	//視点移動(ベクトルの加算)
	//	viewProjection_.eye += move;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバック用表示
	//	debugText_->SetPos(50, 50);
	//	debugText_->Printf(
	//	  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	//}

	////注視点移動処理
	//{
	//	//注視点の移動ベクトル
	//	Vector3 move = {0, 0, 0};

	//	//注視点の移動速さ
	//	const float kTargetSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_LEFT)) {
	//		move.x -= kTargetSpeed;
	//	} else if (input_->PushKey(DIK_RIGHT)) {
	//		move.x += kTargetSpeed;
	//	}

	//	//注視点移動(ベクトルの加算)
	//	viewProjection_.target += move;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバック用表示
	//	debugText_->SetPos(50, 70);
	//	debugText_->Printf(
	//	  "eye:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
	//	  viewProjection_.target.z);
	//}

	////上方向回転処理
	//{
	//	//上方向の回転速さ[ラジアン/flame]
	//	const float kUpRotSpeed = 0.05f;

	//		//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_SPACE)) {
	//		viewAngle += kUpRotSpeed;

	//		// 2PIを超えたら0に戻す
	//		viewAngle = fmod(viewAngle, PI * 2.0f);
	//	}

	//	//上方向ベクトルを計算
	//	viewProjection_.up = {cosf(viewAngle), sinf(viewAngle),0.0f};

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();
	//	//デバック用表示
	//	debugText_->SetPos(50, 90);
	//	debugText_->Printf(
	//	  "eye:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y,
	//	  viewProjection_.up.z);
	//}

	// FoV変更処理
	{
		//上キーで視野角が広がる
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.fovAngleY += 0.01f;
			// PIを上回らないようにする
			if (viewProjection_.fovAngleY > PI) {
				viewProjection_.fovAngleY = PI;
			}
			//下キーで視野角が狭まる
		} else if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.fovAngleY -= 0.01f;
			// 0.01fを下回らないようにする
			if (viewProjection_.fovAngleY < 0.01f) {
				viewProjection_.fovAngleY = 0.01f;
			}
		}

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバック用表示
		debugText_->SetPos(50,110);
		debugText_->Printf("fovAngleY(Degree):%f",(180 / PI) * (viewProjection_.fovAngleY));

	}

	//クリップ距離変更処理
	{
		//上下キーでニアクリップ距離を増減
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.nearZ += 0.0001f;
		} else if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.nearZ -= 0.0001f;
		}

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバック用表示
		debugText_->SetPos(50, 130);
		debugText_->Printf("nearZ:%f", viewProjection_.nearZ);

	}
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

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	for (WorldTransform& worldTransform : worldTransforms_) {
		// 3Dモデル描画
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
	}

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

//ワールド変換行列

//拡大縮小
Matrix4 GameScene::Scale(Vector3 Scale) {

	//スケーリング行列を宣言
	Matrix4 matScale;

	//スケーリング倍率を行列を設定する
	matScale = {Scale.x, 0.0f, 0.0f,    0.0f, 0.0f, Scale.y, 0.0f, 0.0f,
	            0.0f,    0.0f, Scale.z, 0.0f, 0.0f, 0.0f,    0.0f, 1.0f};

	return matScale;
};

//回転X
Matrix4 GameScene::RotX(float roteX) {

	Matrix4 matRotX;

	// X軸回転行列の各要素を設定する
	matRotX = {1.0f, 0.0f,        0.0f,       0.0f, 0.0f, cos(roteX), sin(roteX), 0.0f,
	           0.0f, -sin(roteX), cos(roteX), 0.0f, 0.0f, 0.0f,       0.0f,       1.0f};

	return matRotX;
};

//回転Y
Matrix4 GameScene::RotY(float roteY) {

	Matrix4 matRotY;

	// Y軸回転行列の各要素を設定する

	matRotY = {cos(roteY), 0.0f, -sin(roteY), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	           sin(roteY), 0.0f, cos(roteY),  0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	return matRotY;
};

//回転Z
Matrix4 GameScene::RotZ(float roteZ) {

	Matrix4 matRotZ;

	// Z軸回転行列の各要素を設定する

	matRotZ = {cos(roteZ), sin(roteZ), 0.0f, 0.0f, -sin(roteZ), cos(roteZ), 0.0f, 0.0f,
	           0.0f,       0.0f,       1.0f, 0.0f, 0.0f,        0.0f,       0.0f, 1.0f};

	return matRotZ;
};

//回転合成
Matrix4 GameScene::Rot(Matrix4 RotX, Matrix4 RotY, Matrix4 RotZ) {

	// 合成用回転行列を宣言
	Matrix4 matRot;

	//各軸の回転行列を合成
	matRot = (RotZ *= RotX *= RotY);

	return matRot;
};

//平行移動
Matrix4 GameScene::Trans(Vector3 Trans) {

	//スケーリング行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//スケーリング倍率を行列を設定する
	matTrans = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f,    1.0f,    0.0f,    0.0f,
	            0.0f, 0.0f, 1.0f, 0.0f, Trans.x, Trans.y, Trans.z, 1.0f};

	return matTrans;
};

//ワールド変換行列
Matrix4 GameScene::World(Matrix4 Scale, Matrix4 Rot, Matrix4 Trans) {

	Matrix4 worldTransform_;

	//行列の合成

	//単位行列を代入する
	worldTransform_ = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	                   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.f,  0.0f, 1.0f};

	//掛け算して代入する
	worldTransform_ *= Scale *= Rot *= Trans;

	////行列の転送
	// worldTransform.TransferMatrix();

	return worldTransform_;
};
