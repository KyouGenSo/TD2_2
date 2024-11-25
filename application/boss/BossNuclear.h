#pragma once
#include "vector3.h"
#include <memory>
#include <Object3d.h>
#include "ObjectBase.h"

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

    // 核の位置を取得
    Transform& GetTransform() { return transform_; }

private:
    Transform transform_;    // 核の現在位置
    Vector3 offset_;      // ボスからのオフセット位置

    std::unique_ptr<Object3d> object3d_; // 核の描画用オブジェクト

};

