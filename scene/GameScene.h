#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	Vector3 vertex[8] = {

	  {0.0f, 0.0f, 0.0f},
      {0.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 0.0f},
      {5.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 5.0f},
      {0.0f, 5.0f, 5.0f},
      {5.0f, 5.0f, 5.0f},
      {5.0f, 0.0f, 5.0f}
	};

	int edgeList[12][2] = {
	  {0, 1},
      {1, 2},
      {2, 3},
      {3, 0},
      {0, 4},
      {1, 5},
	  {2, 6},
      {3, 7},
      {4, 5},
      {5, 6},
      {6, 7},
      {7, 4}
    };


	//アフィン変換

	float moveBox[8][4] = {
	  {0, 0, 0, 1},
      {0, 5, 0, 1},
      {5, 5, 0, 1},
      {5, 0, 0, 1},
	  {0, 0, 5, 1},
      {0, 5, 5, 1},
      {5, 5, 5, 1},
      {5, 0, 5, 1}
    };

	float rotateBox[8][4] = {
	  {0, 0, 0, 1},
      {0, 5, 0, 1},
      {5, 5, 0, 1},
      {5, 0, 0, 1},
	  {0, 0, 5, 1},
      {0, 5, 5, 1},
      {5, 5, 5, 1},
      {5, 0, 5, 1}
    };

	float scareBox[8][4] = {
	  {0, 0, 0, 1},
      {0, 5, 0, 1},
      {5, 5, 0, 1},
      {5, 0, 0, 1},
	  {0, 0, 5, 1},
      {0, 5, 5, 1},
      {5, 5, 5, 1},
      {5, 0, 5, 1}
    };
	
	float moveInitialBox[8][4] = {};

	float rotateInitialBox[8][4] = {};

	float scareInitialBox[8][4] = {};


	//基準点を原点に移動する用変数

	//左上の点[0][]の初期値
	float moveInitialPosX = moveBox[0][0];
	float moveInitialPosY = moveBox[0][1];
	float moveInitialPosZ = moveBox[0][2];

	//原点移動用
	float startPointMove[4][4]{};

	//初期値に戻す用
	float initialPointMove[4][4]{};

	//平行移動
	float move[4][4]{};

	//回転
	float rotate[4][4]{};

	//拡大縮小
	float scare[4][4]{};

	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
