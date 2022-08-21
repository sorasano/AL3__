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
	//���[���h�ϊ��f�[�^
	WorldTransform worldtransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//�A�t�B���s��
	Affine* affine_ = nullptr;

};

