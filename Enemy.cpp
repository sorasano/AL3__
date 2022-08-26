#include "Enemy.h"
#include "GameScene.h"

void Enemy::Initialize(Model* model, const Vector3& position) {

	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("enemy.jpeg");

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ�������
	worldtransform_.Initialize();
	//�����Ŏ󂯎�����������W���Z�b�g
	worldtransform_.translation_ = { position.x,position.y,position.z };

	//�A�t�B���s��
	affine_ = new Affine();

	InitializeApproach();

}

void Enemy::Update() {

	if (life <= 0) {
		worldtransform_.translation_ = Vector3(100,100,100);
		//�s��X�V
		worldtransform_.matWorld_ = affine_->World(affine_->Scale(affine_->Scale_), affine_->Rot(affine_->RotX(affine_->Rot_.x), affine_->RotY(affine_->Rot_.y), affine_->RotZ(affine_->Rot_.z)), affine_->Trans(worldtransform_.translation_));
		worldtransform_.TransferMatrix();

		debugText_->SetPos(400,100);
		debugText_->Printf("clear");
	}
	else {

		//�f�X�t���O�̗����������폜
		bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
			return bullet->IsDead();
			});

		Vector3 approachVelocity(0, 0, -0.1);
		Vector3 leaveVelocity(0.1, 0.1, -0.1);
		Vector3 usuallyVelocity(0.1, 0, 0);

		switch (phase_) {
		case Phase::Approach:
		default:
			//�ړ�
			worldtransform_.translation_ += approachVelocity;
			//����̈ʒu�ɓ��B�����痣�E
			//if (worldtransform_.translation_.z < -10.0f) {
			//	phase_ = Phase::Leave;
			//}

			//���˃^�C�}�[�J�E���g�_�E��
			fireTimer--;
			if (fireTimer <= 0) {
				//�e�𔭎�
				TargetFire();
				//���˃^�C�}�[��������
				fireTimer = kFireInterval;
			}
			break;
		case Phase::Leave:
			//�ړ�
			worldtransform_.translation_ += leaveVelocity;
			break;
		case Phase::usually:

			if (worldtransform_.translation_.x <= 20 && usually == 0) {
				worldtransform_.translation_ += usuallyVelocity;
			}
			else {
				usually = 1;
			}

			if (worldtransform_.translation_.x >= -20 && usually == 1) {
				worldtransform_.translation_ -= usuallyVelocity;
			}
			else {
				usually = 0;
			}

			//���˃^�C�}�[�J�E���g�_�E��
			fireTimer--;
			if (fireTimer <= 0) {
				//�e�𔭎�
				Fire();
				//���˃^�C�}�[��������
				fireTimer = kFireInterval;
			}
			break;
		case Phase::wait:
			worldtransform_.translation_ = Vector3(0, 0, 20);
			break;
		}

		if (input_->TriggerKey(DIK_1)) {
			phase_ = Phase::Approach;
		}
		else if (input_->TriggerKey(DIK_2)) {
			phase_ = Phase::usually;
		}
		else if (input_->TriggerKey(DIK_3)) {
			phase_ = Phase::wait;
		}

		//�s��X�V
		worldtransform_.matWorld_ = affine_->World(affine_->Scale(affine_->Scale_), affine_->Rot(affine_->RotX(affine_->Rot_.x), affine_->RotY(affine_->Rot_.y), affine_->RotZ(affine_->Rot_.z)), affine_->Trans(worldtransform_.translation_));
		worldtransform_.TransferMatrix();


		//�e�X�V
		for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
			bullet->Update();
		}
	}

}

void Enemy::Draw(const ViewProjection& viewProjection) {

	//3D���f����`��
	model_->Draw(worldtransform_, viewProjection, textureHandle_);


	//�e�`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}

void Enemy::Fire() {

	assert(player_);

		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		if (phase_ == Phase::Approach) {

			//���L�����̃��[���h���W���擾
			Vector3 playerWorldPos = player_->GetWorldPosition();
			//�G�L�����̃��[���h���W���擾
			Vector3 enemyWorldPos = GetWorldPosition();
			//�G�L���������L�����̍����x�N�g�������߂�
			Vector3 velocity = velocity.sub(enemyWorldPos, playerWorldPos);
			//�x�N�g���̐��K��
			velocity.normalize();
			////�x�N�g���̒����𑬂��ɍ��킹��
			//velocity *= kBulletSpeed;
		}

		//�e�𐶐���������
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(model_, worldtransform_.translation_, velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));

}

void Enemy::TargetFire() {

	assert(player_);

	//���L�����̃��[���h���W���擾
	Vector3 playerWorldPos = player_->GetWorldPosition();
	//�G�L�����̃��[���h���W���擾
	Vector3 enemyWorldPos = GetWorldPosition();
	//�G�L���������L�����̍����x�N�g�������߂�
	Vector3 velocity = velocity.sub(enemyWorldPos, playerWorldPos);
	//�x�N�g���̐��K��
	velocity.normalize();

	//�e�𐶐���������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldtransform_.translation_, velocity);

	//�e��o�^����
	bullets_.push_back(std::move(newBullet));
}

void Enemy::InitializeApproach() {
	//���˃^�C�}�[��������
	fireTimer = kFireInterval;
}

Vector3 Enemy::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldtransform_.matWorld_.m[3][0];
	worldPos.y = worldtransform_.matWorld_.m[3][1];
	worldPos.z = worldtransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() {
	life--;
}