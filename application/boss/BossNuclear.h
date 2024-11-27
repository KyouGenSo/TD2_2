#pragma once
#include "vector3.h"
#include <memory>
#include <Object3d.h>
#include "ObjectBase.h"

class Boss;

class BossNuclear : public ObjectBase
{
public:
	// 核の初期化
	void Initialize(const Vector3& position, const Vector3& offset);

	// 核の更新
	void Update(const Vector3& bossPosition);

	// 核の描画
	void Draw();

	// 衝突処理イベント
	void OnCollision(ObjectBase* objectBase);

	///--------------------------------------------------------------
	///						 
	// 核の位置を取得
	Transform& GetTransform() { return transform_; }
	void SetBoss(Boss* boss) { boss_ = boss; }
	void SetVisible(bool visible) { isVisible_ = visible; }
	bool IsVisible() const { return isVisible_; }

	// 核の位置を設定
	void SetTransform(const Transform& transform) { transform_ = transform; }

	bool IsDestroyed() const { return isDestroyed_; } // 核が破壊されているか取得
	void ResetCore(); // 核をリセットするメソッド

	///--------------------------------------------------------------
	///	
	// パーティクル
	struct Particle {
		std::vector<std::unique_ptr<Object3d>> models; // 破壊時のパーティクル
		std::vector<Vector3> velocities; // パーティクルの速度
		std::vector<float> lifeTimes; // パーティクルの寿命
		std::vector<bool> isDead; // パーティクルが死んでいるかどうか
	};

	void MakeParticle();
	void UpdateParticle();
	void DrawParticle();

	const int MAX_PARTICLE = 25; // パーティクルの最大数

private:
	Transform transform_;    // 核の現在位置
	Vector3 offset_;      // ボスからのオフセット位置

	std::unique_ptr<Object3d> object3d_; // 核の描画用オブジェクト

	float alpha_ = 1.0f;     // 核の透明度（0.0～1.0）

	Boss* boss_ = nullptr; // Bossへの参照

	bool isDestroyed_ = false; // 核が壊れたかどうかを示すフラグ

	bool isVisible_ = false; // 核の可視性

	bool isCollision_ = false; // 衝突したかどうか

	float destructionTimer_ = 0.0f; // 破壊後のタイマー
	const float destructionDuration_ = 3.0f; // 核が消えるまでの時間（秒）

	// パーティクル
	std::unique_ptr<Particle> particle_;

};

