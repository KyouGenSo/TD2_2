#include "Boss.h"
#include "Object3d.h"
#include <cmath>
#include"math.h"


void Boss::Initialize(){
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetModel("Boss.obj");

	//初期位置の設定
	transform_.scale = { 1.0f,1.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,0.0f };
}

void Boss::Update(){
    // BossがPlayerの方向を向くための目標角度を計算
    Vector3 directionToPlayer = playerPosition_ - transform_.translate;
    float targetRotationY = atan2f(directionToPlayer.x, directionToPlayer.z);

    // 現在の回転角度と目標の角度の差を計算
    float angleDifference = targetRotationY - transform_.rotate.y;

    // 角度差がπ（180度）を超える場合、短い方向に回転するよう調整
    if (angleDifference > M_PI) {
        angleDifference -= 2 * static_cast<float>(M_PI);
    }
    else if (angleDifference < -M_PI) {
        angleDifference += 2 * static_cast<float>(M_PI);
    }

    // 最大回転速度を設定し、差分をゆっくり回転
    const float maxRotationSpeed = 0.02f; // 最大回転速度
    if (angleDifference > maxRotationSpeed) {
        transform_.rotate.y += maxRotationSpeed;
    }
    else if (angleDifference < -maxRotationSpeed) {
        transform_.rotate.y -= maxRotationSpeed;
    }
    else {
        // 角度差が小さい場合は目標角度に合わせる
        transform_.rotate.y = targetRotationY;
    }


	object3d_->SetScale(transform_.scale);
	object3d_->SetRotate(transform_.rotate);
	object3d_->SetTranslate(transform_.translate);

	object3d_->Update();
}

void Boss::Draw(){
	object3d_->Draw();
}