#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Affine.h"
#include "EnemyBullet.h"

#include "WinApp.h"
#include "DebugText.h"
#include "Input.h"
#include <memory>
#include <list>
#include "Matrix4.h"

class Player;

class Enemy
{
public:
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void Fire();

	//接近フェーズ初期化
	void InitializeApproach();

	void SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

private:
	//ワールド変換データ
	WorldTransform worldtransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//アフィン行列
	Affine* affine_ = nullptr;

	Input* input_ = nullptr;

	//自キャラ
	Player* player_ = nullptr;

	//行動フェーズ
	enum class Phase {
		Approach,//接近する
		Leave,   //離脱する
	};

	Phase phase_ = Phase::Approach;

private:
	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

public:
	//発射間隔
	static const int kFireInterval = 60;

private:
	//発射タイマー
	int32_t fireTimer = 0;
};

