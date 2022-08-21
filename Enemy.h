#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Affine.h"

class Enemy
{
public:
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);
private:
	//ワールド変換データ
	WorldTransform worldtransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//アフィン行列
	Affine* affine_ = nullptr;

};

