#include "Boss.h"
#include "Object3d.h"
#include <cmath>
#include"math.h"
#include <Input.h>


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


    // HPの更新
    HPUpdate();


	object3d_->SetScale(transform_.scale);
	object3d_->SetRotate(transform_.rotate);
	object3d_->SetTranslate(transform_.translate);

	object3d_->Update();
}

void Boss::Draw(){
	object3d_->Draw();
}

void Boss::HPUpdate()
{
    // HP減らす(デバック用)
    if (Input::GetInstance()->PushKey(DIK_SPACE)) {
        if (hp_ > 0) {
            hp_ -= 1;
            // HPバーの幅を更新
            boxSize.x = static_cast<float>(hp_);
        }
    }

    // HPに応じて色を変更
    float hpRatio = static_cast<float>(hp_) / 1000.0f;
    if (hpRatio > 0.8f) {
        boxColor = Vector4(0.0f, 1.0f, 0.0f, 1.0f); // 緑
    }
    else if (hpRatio > 0.6f) {
        boxColor = Vector4(0.3f, 1.0f, 0.0f, 1.0f); // 黄緑
    }
    else if (hpRatio > 0.4f) {
        boxColor = Vector4(1.0f, 1.0f, 0.0f, 1.0f); // 黄色
    }
    else if (hpRatio > 0.2f) {
        boxColor = Vector4(1.0f, 0.5f, 0.0f, 1.0f); // オレンジ
    }
    else {
        boxColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f); // 赤
    }
}

void Boss::HPDraw()
{
    // 背景用の薄い黒色のボックスを描画
    Vector4 backgroundColor = Vector4(0.0f, 0.0f, 0.0f, 0.5f); // 薄い黒色
    Vector2 backgroundSize = Vector2(1000.0f, boxSize.y);       // 初期サイズの幅と現在の高さ
    Draw2D::GetInstance()->DrawBox(boxPosition, backgroundSize, backgroundColor);

    // HPバーを描画
    Draw2D::GetInstance()->DrawBox(boxPosition, boxSize, boxColor);

    // HPバーの枠を描画
    Vector4 borderColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f); // 黒色

    // 枠の4辺を描画
    Vector2 topLeft = boxPosition;
    Vector2 topRight = Vector2(boxPosition.x + 1000.0f, boxPosition.y); // HP最大値に基づく固定幅
    Vector2 bottomLeft = Vector2(boxPosition.x, boxPosition.y + boxSize.y);
    Vector2 bottomRight = Vector2(boxPosition.x + 1000.0f, boxPosition.y + boxSize.y);

    Draw2D::GetInstance()->DrawLine(topLeft, topRight, borderColor);      // 上辺
    Draw2D::GetInstance()->DrawLine(topRight, bottomRight, borderColor);  // 右辺
    Draw2D::GetInstance()->DrawLine(bottomRight, bottomLeft, borderColor); // 下辺
    Draw2D::GetInstance()->DrawLine(bottomLeft, topLeft, borderColor);    // 左辺
}
