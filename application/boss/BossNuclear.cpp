#define NOMINMAX
#include <cmath>
#include <algorithm> 
#include <random>
#include "BossNuclear.h"
#include "Boss.h"
#include "ModelManager.h"
#include "LightCollision.h"
#include "Object3dBasic.h"

void BossNuclear::Initialize(const Vector3& position, const Vector3& offset)
{
	// プレイヤーモデルの読み込みと設定
	ModelManager::GetInstance()->LoadModel("Core.obj");
	ModelManager::GetInstance()->LoadModel("WhiteBox.obj");

	transform_.translate = position;
	offset_ = offset;

	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetModel("Core.obj");

	alpha_ = 1.0f; // アルファ値を初期化

	// Bossの位置とColliderの位置を同期
	ObjectBase::Init(transform_.translate, transform_.translate, 1.0f);


	//========================================
	// Bossの位置とColliderの位置を同期
	ObjectBase::Init(transform_.translate, transform_.translate, 1.0f);

	// パーティクルの初期化
	particle_ = std::make_unique<Particle>();
}

void BossNuclear::Update(const Vector3& bossPosition)
{
	if (isDestroyed_) {
		// 破壊された場合、タイマーを進める
		destructionTimer_ += 1.0f / 60.0f;
		if (destructionTimer_ >= destructionDuration_) {
			SetVisible(false); // 一定時間後に非表示
		}
		return; // 破壊中は位置を更新しない
	}

	// 通常の処理
	transform_.translate = bossPosition + offset_;
	object3d_->SetTranslate(transform_.translate);
	object3d_->Update();

	// 判定場所の処理
	Vector3 test = transform_.translate + Vector3(0.0f, 0.1f, 0.0f);
	ObjectBase::Update(transform_.translate, test);

	UpdateParticle();

	// 死亡したパーティクルを削除
	for (size_t i = 0; i < particle_->models.size(); i++) {
		if (particle_->isDead[i]) {
			particle_->models.erase(particle_->models.begin() + i);
			particle_->velocities.erase(particle_->velocities.begin() + i);
			particle_->lifeTimes.erase(particle_->lifeTimes.begin() + i);
			particle_->isDead.erase(particle_->isDead.begin() + i);
			particle_->models.shrink_to_fit();
			particle_->velocities.shrink_to_fit();
			particle_->lifeTimes.shrink_to_fit();
			particle_->isDead.shrink_to_fit();
		}
	}
}

void BossNuclear::Draw()
{
	if (object3d_ && alpha_ > 0.0f && isVisible_) {
		object3d_->SetAlpha(alpha_);
		object3d_->Draw();
	}

	// パーティクルの描画
	if (isCollision_)
	DrawParticle();
}

void BossNuclear::OnCollision(ObjectBase* objectBase) {
	if (!isVisible_ || isDestroyed_) return; // 可視状態でない、または既に破壊済みの場合は無視

	// 通常の衝突処理
	if (dynamic_cast<LightCollision*>(objectBase) != nullptr) {
		collider_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		alpha_ = std::max(0.0f, alpha_ - 0.02f);
		isCollision_ = true;
		MakeParticle();

		if (alpha_ <= 0.0f) {
			isDestroyed_ = true; // 破壊フラグを立てる
			isCollision_ = false; // 衝突フラグをリセット
			destructionTimer_ = 0.0f; // タイマー初期化
			boss_->DecreaseHP(50); // ボスのHPを減少
		}
	}
}

void BossNuclear::ResetCore()
{
	isDestroyed_ = false;       // 破壊フラグをリセット
	destructionTimer_ = 0.0f;   // タイマーをリセット
	alpha_ = 1.0f;              // アルファ値をリセット
	SetVisible(true);           // コアを表示
}

void BossNuclear::MakeParticle()
{
	if (particle_->models.size() >= MAX_PARTICLE) return;

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

	// パーティクルの生成
	auto model = std::make_unique<Object3d>();
	model->Initialize();
	model->SetModel("WhiteBox.obj");
	model->SetTranslate(transform_.translate);
	model->SetScale(Vector3(0.8f, 0.8f, 0.8f));
	model->SetRotate(Vector3(0.0f, 0.0f, 0.0f));
	model->Update();

	particle_->models.push_back(std::move(model));
	particle_->velocities.push_back(Vector3(dist(mt), 0.2f, dist(mt)) * 0.05f);
	particle_->lifeTimes.push_back(1.0f);
	particle_->isDead.push_back(false);
}

void BossNuclear::UpdateParticle()
{
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

	// パーティクルの移動と更新
	for (size_t i = 0; i < particle_->models.size(); i++) {
		if (particle_->isDead[i]) continue; // 死亡しているパーティクルはスキップ

		// パーティクルの移動
		auto& model = particle_->models[i];
		particle_->velocities[i] += Vector3(dist(mt), 0.2f, dist(mt)) * 0.05f; // ランダムに変化
		model->SetTranslate(model->GetTranslate() + particle_->velocities[i]);
		model->Update();

		// パーティクルの寿命を減少させる
		particle_->lifeTimes[i] -= 1.0f / 60.0f;
		if (particle_->lifeTimes[i] <= 0.0f) {
			particle_->isDead[i] = true; // 寿命が尽きたら死亡フラグを立てる
		}
	}
}

void BossNuclear::DrawParticle()
{
	for (size_t i = 0; i < particle_->models.size(); i++) {
		if (particle_->isDead[i]) continue;

		particle_->models[i]->Draw();
	}
}
