#include "Boss.h"
#include "Object3d.h"
#include <cmath>
#include"math.h"
#include <Input.h>
#include "AttackPhase1.h"
#include "AttackPhase2.h"
#include "AttackPhase3.h"
#include "AttackPhase4.h"
#include "AttackPhase5.h"
#include <memory>
#include "Player.h"
#include <random>
#include <SceneManager.h>
#include <iostream>

void Boss::Initialize() {
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetModel("Boss.obj");

	//初期位置の設定
	transform_.scale = { 1.0f,1.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,0.0f };

	// 核の初期化（5つ）
	cores_.resize(5);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distRadius(0.0f, 10.0f);
	std::uniform_real_distribution<float> distAngle(0.0f, 2 * static_cast<float>(M_PI));
	std::uniform_real_distribution<float> distHeight(0.0f, 20.0f);

	// 核のランダムな配置
	for (auto& core : cores_) {
		core = std::make_unique<BossNuclear>();
		float radius = distRadius(gen);
		float angle = distAngle(gen);
		float height = distHeight(gen);
		Vector3 offset = { radius * cos(angle), height, radius * sin(angle) };
		core->Initialize(transform_.translate, offset);
		core->SetBoss(this); // Bossの参照を設定
		core->SetVisible(false); // 初期状態では不可視
	}

	ChangeState(std::make_unique<AttackPhase1>(this)); // 初期状態を設定

	//========================================
	// Bossの位置とColliderの位置を同期
	ObjectBase::Init(transform_.translate, transform_.translate, 10.0f);
	collider_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//particle->Initialize(dxCommon_);
	/*particle = std::make_unique<ParticleManager>();
	particle->CreateParticleGroup("particle", "resources/circle.png", ParticleManager::BlendMode::kBlendModeAdd);*/
	//ParticleManager::GetInstance()->CreateParticleGroup("particle", "circle.png", ParticleManager::BlendMode::kBlendModeAdd);
	//particle->CreateParticleGroup("particle", "circle.png", ParticleManager::BlendMode::kBlendModeAdd);
}

void Boss::Update() {
	// ライトとの衝突状態をチェック
	if (isLightHitting_) {
		lightHitDuration_ += 1.0f / 60.0f; // 衝突中は時間を加算
		if (lightHitDuration_ >= 5.0f) {
			lightHitDuration_ = 5.0f;
			if (!coresAreVisible_) {
				coresAreVisible_ = true;
				for (auto& core : cores_) {
					core->SetVisible(true); // 核を表示
				}
			}
		}
	} else {
		//lightHitDuration_ -= 1.0f / 60.0f; // 衝突していない場合は減少
		//
		lightHitDuration_ = 0.0f;
		//}
	}

	// フラグをリセット（次のフレームで再評価）
	isLightHitting_ = false;

	// 状態に応じた処理を実行
	// 現在のフェーズに対応する関数を呼び出す
	(this->*spFuncTable[static_cast<int>(phase_)])();

	// HPの更新
	HPUpdate();

	// HPが0以下の場合、通常のフェーズからDownフェーズへ移行
	if (hp_ <= 0 && phase_ != Phase::Down) {
		phase_ = Phase::Down;
		std::cout << "Phase changed to Down" << std::endl; // デバッグログ
	}

	// ステートの更新
	if (state_) {
		state_->Update();
	}

	// パーティクルの更新
	UpdateParticles();

	// コアの状態チェック
	allCoresDestroyed_ = std::all_of(cores_.begin(), cores_.end(), [](const std::unique_ptr<BossNuclear>& core) {
		return core->IsVisible() == false;
		});

	// 全てのコアが破壊された場合の再生成処理
	if (allCoresDestroyed_) {
		coreRespawnTimer_ += 1.0f / 60.0f; // フレーム単位でタイマー進行
		if (coreRespawnTimer_ >= 3.0f) {
			RespawnCores();
			coreRespawnTimer_ = 0.0f; // タイマーリセット
			allCoresDestroyed_ = false; // 状態リセット
		}
	}

	// 核の更新
	for (auto& core : cores_) {
		core->Update(transform_.translate);
	}

	object3d_->SetScale(transform_.scale);
	object3d_->SetRotate(transform_.rotate);
	object3d_->SetTranslate(transform_.translate);

	object3d_->Update();

	//========================================
	// 判定場所の処理
	Vector3 test = transform_.translate + Vector3(0.0f, 30.0f, 0.0f);
	ObjectBase::Update(transform_.translate, test);
}

void Boss::Move() {
	// BossがPlayerの方向を向くための目標角度を計算
	Vector3 directionToPlayer = playerPosition_ - transform_.translate;
	float targetRotationY = atan2f(directionToPlayer.x, directionToPlayer.z);

	// 現在の回転角度と目標の角度の差を計算
	float angleDifference = targetRotationY - transform_.rotate.y;

	// 角度差がπ（180度）を超える場合、短い方向に回転するよう調整
	if (angleDifference > M_PI) {
		angleDifference -= 2 * static_cast<float>(M_PI);
	} else if (angleDifference < -M_PI) {
		angleDifference += 2 * static_cast<float>(M_PI);
	}

	// 最大回転速度を設定し、差分をゆっくり回転
	const float maxRotationSpeed = 0.02f; // 最大回転速度
	if (angleDifference > maxRotationSpeed) {
		transform_.rotate.y += maxRotationSpeed;
	} else if (angleDifference < -maxRotationSpeed) {
		transform_.rotate.y -= maxRotationSpeed;
	} else {
		// 角度差が小さい場合は目標角度に合わせる
		transform_.rotate.y = targetRotationY;
	}
}

void Boss::Draw() {
	// 核の描画
	for (auto& core : cores_) {
		core->Draw();
	}

	// ステートの更新
	if (state_) {
		state_->Draw();
	}

	object3d_->Draw();

	// パーティクルの描画
	DrawParticles();

	//ParticleManager::GetInstance()->Draw();

}

void Boss::DrawImGui() {
#ifdef _DEBUG
	ImGui::Begin("phase");
	// フェーズの変更
	float hpRatio = static_cast<float>(hp_) / 1000.0f;
	if (hpRatio > 0.9f) {
		ImGui::Text("phase1");
	} else if (hpRatio > 0.75f) {
		ImGui::Text("phase2");
	} else if (hpRatio > 0.5f) {
		ImGui::Text("phase3");
	} else if (hpRatio > 0.2f) {
		ImGui::Text("phase4");
	} else if (hpRatio <= 0.2f) {
		ImGui::Text("phase5");
	}
	ImGui::DragFloat("duration", &lightHitDuration_);
	ImGui::End(); 
#endif // DEBUG

}

void Boss::HPUpdate() {
	// HPバーの幅を更新（HPの最大値を1000と仮定）
	const float maxHP = 1000.0f;
	boxSize.x = (static_cast<float>(hp_) / maxHP) * 1000.0f;

	// HPバーの色を変更
	float hpRatio = static_cast<float>(hp_) / maxHP;
	if (hpRatio > 0.9f) {
		boxColor = Vector4(0.0f, 1.0f, 0.0f, 1.0f); // 緑
	} else if (hpRatio > 0.7f) {
		boxColor = Vector4(0.3f, 1.0f, 0.0f, 1.0f); // 黄緑
	} else if (hpRatio > 0.5f) {
		boxColor = Vector4(1.0f, 1.0f, 0.0f, 1.0f); // 黄色
	} else if (hpRatio > 0.2f) {
		boxColor = Vector4(1.0f, 0.5f, 0.0f, 1.0f); // オレンジ
	} else {
		boxColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f); // 赤
	}
}	


void Boss::HPDraw() {
	// 背景用の薄い黒色のボックスを描画
	Vector4 backgroundColor = Vector4(0.0f, 0.0f, 0.0f, 0.5f); // 薄い黒色
	Vector2 backgroundSize = Vector2(1000.0f, boxSize.y);      // 初期サイズの幅と現在の高さ
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

void Boss::ChangeState(std::unique_ptr<BossAttackBaseState> state) {
	state_ = std::move(state);
}

std::vector<std::unique_ptr<BossNuclear>>& Boss::GetCores()
{
	return cores_;
}

///=============================================================================
///							衝突判定イベント
void Boss::OnCollision(ObjectBase* objectBase) {
	if (dynamic_cast<LightCollision*>(objectBase) != nullptr) {
		collider_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		isLightHitting_ = true; // 衝突状態を記録
	}
}

void Boss::RespawnCores()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distRadius(0.0f, 12.0f);
	std::uniform_real_distribution<float> distAngle(0.0f, 2 * static_cast<float>(M_PI));
	std::uniform_real_distribution<float> distHeight(0.0f, 12.0f);

	cores_.clear(); // 既存のコアをクリア

	for (int i = 0; i < 5; ++i) {
		auto core = std::make_unique<BossNuclear>();
		float radius = distRadius(gen);
		float angle = distAngle(gen);
		float height = distHeight(gen);
		Vector3 offset = { radius * cos(angle), height, radius * sin(angle) };
		core->Initialize(transform_.translate, offset);
		core->SetBoss(this);
		core->SetVisible(true); // 再生成時は表示
		cores_.push_back(std::move(core));
	}
}

// メンバ関数ポインタのテーブルの実体
void (Boss::* Boss::spFuncTable[])() = {
	&Boss::Usually,
	&Boss::Down,	
	&Boss::GettingUp
};

void Boss::Usually() {
	// 通常の動き
	Move();

	//// ステートの更新
	/*if (state_) {
		state_->Update();
	}*/

	if (hp_ <= 0)
	{
		phase_ = Phase::Down;
	}

	/*if(Input::GetInstance()->PushKey(DIK_V)) {
		phase_ = Phase::Down;
	}*/
}

void Boss::Down() {
	static float easingTime = 0.0f;
	static int waitTimer = 0;
	const float fallDuration = 2.0f;
	const float bounceDuration = 1.0f;
	const float maxRotation = static_cast<float>(M_PI) / 2.0f;

	// アニメーションがまだ終了していない場合
	if (!hasBounced_) {
		// 倒れるアニメーション
		if (easingTime < fallDuration) {
			easingTime += 0.02f;
			float t = easingTime / fallDuration;
			if (t > 1.0f) t = 1.0f;
			float easedT = EaseInExpo(t);
			transform_.rotate.x = maxRotation * easedT;
		}
		// バウンドアニメーション
		else if (easingTime < fallDuration + bounceDuration) {
			easingTime += 0.02f;
			float t = (easingTime - fallDuration) / bounceDuration;
			if (t > 1.0f) t = 1.0f;
			float easedT = EaseOutBounce(t);
			transform_.rotate.x = maxRotation - (maxRotation * 0.1f * easedT);
		}
		// アニメーション終了
		else {
			hasBounced_ = true; // アニメーション終了フラグを設定
			easingTime = 0.0f;  // タイマーリセット
		}
	}
	// アニメーション終了後のフェードアウト処理
	else {
		// 待機タイマーを進行
		waitTimer_++;
		if (waitTimer_ >= 60) { // 60フレーム後にパーティクル生成
			for (int i = 0; i < 5; ++i) { // 5つのパーティクルを生成
				CreateParticle(transform_.translate);
			}

			alpha_ -= 0.01f;
			if (alpha_ <= 0.0f) {
				alpha_ = 0.0f;
				SceneManager::GetInstance()->ChangeScene("clear");
			}
			object3d_->SetAlpha(alpha_);
		}
	}
}



void Boss::GettingUp() {
	// イージング用のタイマー
	static float easingTime = 0.0f; // 起き上がる時間
	const float getUpDuration = 10.0f; // 起き上がりにかける時間（秒）

	// イージングタイマーの進行
	if (easingTime < getUpDuration) {
		easingTime += 0.02f; // 時間を進める
	}

	// 正規化時間 t を計算（0.0f～1.0f）
	float t = easingTime / getUpDuration;
	if (t > 1.0f) t = 1.0f;

	// イージング値を取得（起き上がる動き）
	float easedT = EaseInOut(t);

	// 回転角の更新（ゆっくり元に戻す）
	transform_.rotate.x = (1.0f - easedT) * (static_cast<float>(M_PI) / 2.0f);

	// 位置の更新（元の位置に戻す）
	transform_.translate.x *= (1.0f - easedT);

	//// 起き上がりが完了したら通常状態に戻す
	//if (t >= 1.0f) {
	//	transform_.rotate.x = 0.0f;  // 回転を完全にリセット
	//	transform_.translate.x = 0.0f; // 位置を完全にリセット
	//	easingTime = 0.0f;           // タイマーをリセット
	//	phase_ = Phase::Usually;     // 通常状態に戻す
	//}
}

// パーティクルを生成
void Boss::CreateParticle(const Vector3& basePosition) {
	// ランダムな移動量を生成
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distX(-2.1f, 2.1f); // X軸のランダム範囲
	std::uniform_real_distribution<float> distZ(-2.1f, 2.1f); // Z軸のランダム範囲
	std::uniform_real_distribution<float> distY(0.5f, 20.8f);  // Y軸の上方向範囲

	// パーティクルの位置
	Vector3 position = basePosition;
	position.x += distX(gen); // X軸をランダムに
	position.z += distZ(gen); // Z軸をランダムに
	position.y += distY(gen); // Y軸はプラス方向

	// パーティクルオブジェクトを生成
	std::unique_ptr<Object3d> particleObject = std::make_unique<Object3d>();
	particleObject->Initialize();
	particleObject->SetModel("Particle.obj"); // パーティクル用モデル
	particleObject->SetTranslate(position);
	particleObject->SetScale({ 0.2f, 0.2f, 0.2f }); // スケール調整
	particleObject->Update();

	// 最大個数を超えた場合は古いパーティクルを削除
	if (particles_.size() >= kMaxParticles) {
		particles_.erase(particles_.begin());
	}

	// パーティクルをリストに追加
	particles_.emplace_back(std::move(particleObject));
}

// パーティクルの更新処理
void Boss::UpdateParticles() {
	for (auto it = particles_.begin(); it != particles_.end(); ) {
		Vector3 position = (*it)->GetTranslate();
		position.y += 0.1f; // Y軸方向に徐々に下がる

		// パーティクルの更新
		(*it)->SetTranslate(position);
		(*it)->Update();

		// パーティクルが画面外または条件を満たさない場合は削除（今回は削除しない）
		++it;
	}
}

// パーティクルの描画処理
void Boss::DrawParticles() {
	for (const auto& particle : particles_) {
		particle->Draw();
	}
}