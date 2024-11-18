#include "Player.h"
#include "Input.h"
#include "Model.h"
#include "ModelManager.h"
#include "Object3dBasic.h"
#include "Camera.h"
#include <cmath>

void Player::Initialize(Boss* boss) {
	// スケール、回転、位置の初期設定
	transform_.scale = { 5.0f, 5.0f, 5.0f };
	transform_.rotate = { 0.0f, 0.0f, 0.0f };
	transform_.translate = { 0.0f, 0.0f, -13.0f };

	boss_ = boss; // Boss のポインタを設定


	//プレイヤーの位置とColliderの位置を同期
	ObjectBase::Init(transform_.translate, transform_.translate,16.0f);
}

void Player::Update() {

	// Boss が存在しない場合、処理をスキップ
	//（Boss の位置に依存した処理があるため、nullptr の場合にクラッシュを防ぐための安全対策）
	if(boss_ == nullptr) return;

	// 移動処理
	Move();

	// BossにPlayerの位置を通知
	boss_->SetPlayerPosition(transform_.translate);

	// モデルの更新
	object3d_->SetScale(transform_.scale);
	object3d_->SetRotate(transform_.rotate);
	object3d_->SetTranslate(transform_.translate);
	object3d_->Update();

	// 追従カメラ
	FollowCamera();


	Vector3 test = transform_.translate + Vector3(0.0f, 32.0f, 0.0f);
	ObjectBase::Update(transform_.translate, test);
}

void Player::Draw() {
	// モデルの描画
	object3d_->Draw();

	ObjectBase::Draw();
}

void Player::Move() {
	// プレイヤーの左右移動 (Boss の周りを回転)
	if(Input::GetInstance()->PushKey(DIK_A)) {
		angle_ -= rotationSpeed_; // 左回転
	}
	if(Input::GetInstance()->PushKey(DIK_D)) {
		angle_ += rotationSpeed_; // 右回転
	}

	// Boss の位置を中心に円状に移動
	const Transform& bossTransform = boss_->GetTransform();
	transform_.translate.x = bossTransform.translate.x + radius_ * cosf(angle_);
	transform_.translate.z = bossTransform.translate.z + radius_ * sinf(angle_);

	// Boss の方向を向くための角度計算
	Vector3 directionToBoss = bossTransform.translate - transform_.translate;
	transform_.rotate.y = atan2f(directionToBoss.x, directionToBoss.z);

	// ジャンプ処理
	if(Input::GetInstance()->PushKey(DIK_W) && !isJumping_) {
		isJumping_ = true;
		jumpVelocity_ = jumpPower_;
	}

	if(isJumping_) {
		transform_.translate.y += jumpVelocity_;
		jumpVelocity_ += gravity_;

		if(transform_.translate.y <= 0.0f) {
			transform_.translate.y = 0.0f;
			isJumping_ = false;
			jumpVelocity_ = 0.0f;
		}
	}
}

void Player::FollowCamera() {
	// プレイヤーの背後に追従するカメラ
	Camera* camera = Object3dBasic::GetInstance()->GetCamera();
	if(camera) {
		// プレイヤーの回転角度に合わせたカメラのオフセットを計算
		float cameraDistance = 25.0f; // プレイヤーからカメラまでの距離を少し遠めに
		float cameraHeight = 2.0f;    // カメラの高さを低めに設定

		// プレイヤーの背後の位置を計算
		float offsetX = cameraDistance * sinf(transform_.rotate.y);
		float offsetZ = cameraDistance * cosf(transform_.rotate.y);

		Vector3 cameraPos;
		cameraPos.x = transform_.translate.x - offsetX;
		cameraPos.y = transform_.translate.y + cameraHeight;
		cameraPos.z = transform_.translate.z - offsetZ;

		camera->SetTranslate(cameraPos);

		// カメラが見上げるように角度を調整
		Vector3 lookAt = transform_.translate;
		Vector3 directionToPlayer = lookAt - cameraPos;
		float cameraYAngle = atan2f(directionToPlayer.x, directionToPlayer.z);

		// X軸の回転角を負の値にしてカメラを見上げるように設定
		camera->SetRotate(Vector3(-0.08f, cameraYAngle, 0.0f));
	}
}

void Player::OnCollision(ObjectBase* objectBase) {
	// 衝突処理
	//ダイナミックキャストにて判定
	if(dynamic_cast<Boss*>( objectBase )) {
		// Boss との衝突処理
	} else {
		// その他のオブジェクトとの衝突処理
	}
}
