#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>

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

	//ワールド変換行列

	//拡大縮小
	Matrix4 Scale(Vector3 Scale);

	//回転X
	Matrix4 RotX(float roteX);
	
	//回転Y
	Matrix4 RotY(float roteY);

	//回転Z
	Matrix4 RotZ(float roteZ);

	//回転合成
	Matrix4 Rot(Matrix4 RotX, Matrix4 RotY, Matrix4 RotZ);

	//平行移動
	Matrix4 Trans(Vector3 Trans);

	//ワールド変換行列
	Matrix4 World(Matrix4 Scale, Matrix4 Rot, Matrix4 Trans);

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	float viewAngle = 0.0f;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// 3Dモデル
	Model* model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransforms_[100];
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
