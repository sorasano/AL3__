#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() { delete debugCamera_; }

const double PI = 3.14159;

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//デバッグカメラ
	debugCamera_ = new DebugCamera(1280, 780);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//ボックスの初期値

	//アフィン変換

	
	//基準点を原点に移動する用変数

	//左上の点[0][]の初期値
	float initialPosX = moveBox[0][0];
	float initialPosY = moveBox[0][1];
	float initialPosZ = moveBox[0][2];

	//原点移動用
	float startPointMove[4][4]{
	  {1.0f, 0.0f, 0.0f, -initialPosX},
	  {0.0f, 1.0f, 0.0f, -initialPosY},
	  {0.0f, 0.0f, 1.0f, -initialPosZ},
	  {0.0f, 0.0f, 0.0f, 1.0f        }
    };

	//初期値に戻す用
	float initialPointMove[4][4]{
	  {1.0f, 0.0f, 0.0f, initialPosX},
	  {0.0f, 1.0f, 0.0f, initialPosY},
	  {0.0f, 0.0f, 1.0f, initialPosY},
	  {0.0f, 0.0f, 0.0f, 1.0f       }
    };

	//----------平行移動-----------\\

	float move[4][4]{
	  {1.0f, 0.0f, 0.0f, 5.0f},
	  {0.0f, 1.0f, 0.0f, 5.0f},
	  {0.0f, 0.0f, 1.0f, 5.0f},
	  {0.0f, 0.0f, 0.0f, 1.0f}
    };

	//-----------回転-----------\\

	float rotate[4][4]{
	  {cos(PI / 4), -sin(PI / 4), 0.0f, 0.0f},
	  {sin(PI / 4), cos(PI / 4),  0.0f, 0.0f},
	  {0.0f,        0.0f,         1.0f, 0.0f},
	  {0.0f,        0.0f,         0.0f, 1.0f}
    };

	//------------拡大縮小------------\\

	float scare[4][4]{
	  {2.0f, 0.0f, 0.0f, 0.0f},
	  {0.0f, 2.0f, 0.0f, 0.0f},
	  {0.0f, 0.0f, 2.0f, 0.0f},
	  {0.0f, 0.0f, 0.0f, 1.0f}
    };

	//------------平行移動-----------\\
}

	for (int i = 0; i < 8; i++) {

		// moveBoxと同じ値を他の変数に代入
		for (int j = 0; j < 4; j++) {
			moveInitialBox[i][j] = moveBox[i][j];
		}

		moveBox[i][0] = move[0][0] * moveInitialBox[i][0] + move[0][1] * moveInitialBox[i][1] +
		                move[0][2] * moveInitialBox[i][2] + move[0][3] * moveInitialBox[i][3];

		moveBox[i][1] = move[1][0] * moveInitialBox[i][0] + move[1][1] * moveInitialBox[i][1] +
		                move[1][2] * moveInitialBox[i][2] + move[1][3] * moveInitialBox[i][3];

		moveBox[i][2] = move[2][0] * moveInitialBox[i][0] + move[2][1] * moveInitialBox[i][1] +
		                move[2][2] * moveInitialBox[i][2] + move[2][3] * moveInitialBox[i][3];

		moveBox[i][3] = move[3][0] * moveInitialBox[i][0] + move[3][1] * moveInitialBox[i][1] +
		                move[3][2] * moveInitialBox[i][2] + move[3][3] * moveInitialBox[i][3];

	}

	//------------回転-----------\\


	for (int i = 0; i < 8; i++) {

		// rotateBoxと同じ値を他の変数に代入
		for (int j = 0; j < 4; j++) {
			rotateInitialBox[i][j] = rotateBox[i][j];
		}

		//// 1 基準点を原点に移動

		//rotateBox[i][0] = startPointMove[0][0] * rotateInitialBox[i][0] +
		//                  startPointMove[0][1] * rotateInitialBox[i][1] +
		//                  startPointMove[0][2] * rotateInitialBox[i][2] +
		//                  startPointMove[0][3] * rotateInitialBox[i][3];

		//rotateBox[i][1] = startPointMove[1][0] * rotateInitialBox[i][0] +
		//                  startPointMove[1][1] * rotateInitialBox[i][1] +
		//                  startPointMove[1][2] * rotateInitialBox[i][2] +
		//                  startPointMove[1][3] * rotateInitialBox[i][3];

		//rotateBox[i][2] = startPointMove[2][0] * rotateInitialBox[i][0] +
		//                  startPointMove[2][1] * rotateInitialBox[i][1] +
		//                  startPointMove[2][2] * rotateInitialBox[i][2] +
		//                  startPointMove[2][3] * rotateInitialBox[i][3];

		//rotateBox[i][3] = startPointMove[3][0] * rotateInitialBox[i][0] +
		//                  startPointMove[3][1] * rotateInitialBox[i][1] +
		//                  startPointMove[3][2] * rotateInitialBox[i][2] +
		//                  startPointMove[3][3] * rotateInitialBox[i][3];

		// 2 回転

		rotateBox[i][0] =
		  rotate[0][0] * rotateInitialBox[i][0] + rotate[0][1] * rotateInitialBox[i][1] +
		  rotate[0][2] * rotateInitialBox[i][2] + rotate[0][3] * rotateInitialBox[i][3];

		rotateBox[i][1] =
		  rotate[1][0] * rotateInitialBox[i][0] + rotate[1][1] * rotateInitialBox[i][1] +
		  rotate[1][2] * rotateInitialBox[i][2] + rotate[1][3] * rotateInitialBox[i][3];

		rotateBox[i][2] =
		  rotate[2][0] * rotateInitialBox[i][0] + rotate[2][1] * rotateInitialBox[i][1] +
		  rotate[2][2] * rotateInitialBox[i][2] + rotate[2][3] * rotateInitialBox[i][3];

		rotateBox[i][3] =
		  rotate[3][0] * rotateInitialBox[i][0] + rotate[3][1] * rotateInitialBox[i][1] +
		  rotate[3][2] * rotateInitialBox[i][2] + rotate[3][3] * rotateInitialBox[i][3];

		//// 3 元の位置に戻す

		//rotateBox[i][0] = initialPointMove[0][0] * rotateInitialBox[i][0] +
		//                  initialPointMove[0][1] * rotateInitialBox[i][1] +
		//                  initialPointMove[0][2] * rotateInitialBox[i][2] +
		//                  initialPointMove[0][3] * rotateInitialBox[i][3];

		//rotateBox[i][1] = initialPointMove[1][0] * rotateInitialBox[i][0] +
		//                  initialPointMove[1][1] * rotateInitialBox[i][1] +
		//                  initialPointMove[1][2] * rotateInitialBox[i][2] +
		//                  initialPointMove[1][3] * rotateInitialBox[i][3];

		//rotateBox[i][2] = initialPointMove[2][0] * rotateInitialBox[i][0] +
		//                  initialPointMove[2][1] * rotateInitialBox[i][1] +
		//                  initialPointMove[2][2] * rotateInitialBox[i][2] +
		//                  initialPointMove[2][3] * rotateInitialBox[i][3];

		//rotateBox[i][3] = initialPointMove[3][0] * rotateInitialBox[i][0] +
		//                  initialPointMove[3][1] * rotateInitialBox[i][1] +
		//                  initialPointMove[3][2] * rotateInitialBox[i][2] +
		//                  initialPointMove[3][3] * rotateInitialBox[i][3];
	}

	//-------------拡大-------------\\

	for (int i = 0; i < 8; i++) {

		//  scareBoxと同じ値を他の変数に代入
		for (int j = 0; j < 4; j++) {
			scareInitialBox[i][j] = scareBox[i][j];
		}

		//// 1 基準点を原点に移動

		//scareBox[i][0] = startPointMove[0][0] * scareInitialBox[i][0] +
		//                     startPointMove[0][1] * scareInitialBox[i][1] +
		//                     startPointMove[0][2] * scareInitialBox[i][2] +
		//                     startPointMove[0][3] * scareInitialBox[i][3];

		//scareBox[i][1] = startPointMove[1][0] * scareInitialBox[i][0] +
		//                     startPointMove[1][1] * scareInitialBox[i][1] +
		//                     startPointMove[1][2] * scareInitialBox[i][2] +
		//                     startPointMove[1][3] * scareInitialBox[i][3];

		//scareBox[i][2] = startPointMove[2][0] * scareInitialBox[i][0] +
		//                     startPointMove[2][1] * scareInitialBox[i][1] +
		//                     startPointMove[2][2] * scareInitialBox[i][2] +
		//                     startPointMove[2][3] * scareInitialBox[i][3];

		//scareBox[i][3] = startPointMove[3][0] * scareInitialBox[i][0] +
		//                     startPointMove[3][1] * scareInitialBox[i][1] +
		//                     startPointMove[3][2] * scareInitialBox[i][2] +
		//                     startPointMove[3][3] * scareInitialBox[i][3];

		// 2 拡大縮小

		scareBox[i][0] = scare[0][0] * scareInitialBox[i][0] +
		                     scare[0][1] * scareInitialBox[i][1] +
		                     scare[0][2] * scareInitialBox[i][2] +
		                     scare[0][3] * scareInitialBox[i][3];

		scareBox[i][1] = scare[1][0] * scareInitialBox[i][0] +
		                     scare[1][1] * scareInitialBox[i][1] +
		                     scare[1][2] * scareInitialBox[i][2] +
		                     scare[1][3] * scareInitialBox[i][3];

		scareBox[i][2] = scare[2][0] * scareInitialBox[i][0] +
		                     scare[2][1] * scareInitialBox[i][1] +
		                     scare[2][2] * scareInitialBox[i][2] +
		                     scare[2][3] * scareInitialBox[i][3];

		scareBox[i][3] = scare[3][0] * scareInitialBox[i][0] +
		                     scare[3][1] * scareInitialBox[i][1] +
		                     scare[3][2] * scareInitialBox[i][2] +
		                     scare[3][3] * scareInitialBox[i][3];

		////	//3 元の位置に戻す

		//scareBox[i][0] = initialPointMove[0][0] * scareInitialBox[i][0] +
		//                     initialPointMove[0][1] * scareInitialBox[i][1] +
		//                     initialPointMove[0][2] * scareInitialBox[i][2] +
		//                     initialPointMove[0][3] * scareInitialBox[i][3];

		//scareBox[i][1] = initialPointMove[1][0] * scareInitialBox[i][0] +
		//                     initialPointMove[1][1] * scareInitialBox[i][1] +
		//                     initialPointMove[1][2] * scareInitialBox[i][2] +
		//                     initialPointMove[1][3] * scareInitialBox[i][3];

		//scareBox[i][2] = initialPointMove[2][0] * scareInitialBox[i][0] +
		//                     initialPointMove[2][1] * scareInitialBox[i][1] +
		//                     initialPointMove[2][2] * scareInitialBox[i][2] +
		//                     initialPointMove[2][3] * scareInitialBox[i][3];

		//scareBox[i][3] = initialPointMove[3][0] * scareInitialBox[i][0] +
		//                     initialPointMove[3][1] * scareInitialBox[i][1] +
		//                     initialPointMove[3][2] * scareInitialBox[i][2] +
		//                     initialPointMove[3][3] * scareInitialBox[i][3];
	}
}

void GameScene::Update() {

	//デバックカメラの更新
	debugCamera_->Update();

	 debugText_->Printf(
	   "box:(%f,%f,%f)", moveBox[1][0], moveBox[1][1], moveBox[1][2]);

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

	// 3Dオブジェクト描画後処理

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)

	for (int i = 0; i < 12; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  vertex[edgeList[i][0]], vertex[edgeList[i][1]], Vector4(1, 1, 1, 1));
	}

	//平行移動
	for (int i = 0; i < 12; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  Vector3(
		    moveBox[edgeList[i][0]][0], moveBox[edgeList[i][0]][1], moveBox[edgeList[i][0]][2]),
		  Vector3(
		    moveBox[edgeList[i][1]][0], moveBox[edgeList[i][1]][1], moveBox[edgeList[i][1]][2]),
		  Vector4(255, 0, 0, 1));
	}

	//回転
	for (int i = 0; i < 12; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  Vector3(
		    rotateBox[edgeList[i][0]][0], rotateBox[edgeList[i][0]][1],
		    rotateBox[edgeList[i][0]][2]),
		  Vector3(
		    rotateBox[edgeList[i][1]][0], rotateBox[edgeList[i][1]][1],
		    rotateBox[edgeList[i][1]][2]),
		  Vector4(0, 255, 0, 1));
	}

	//拡大縮小
	for (int i = 0; i < 12; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  Vector3(
		    scareBox[edgeList[i][0]][0], scareBox[edgeList[i][0]][1], scareBox[edgeList[i][0]][2]),
		  Vector3(
		    scareBox[edgeList[i][1]][0], scareBox[edgeList[i][1]][1], scareBox[edgeList[i][1]][2]),
		  Vector4(0, 0, 255, 1));
	}
	
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
