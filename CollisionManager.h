#pragma once
#include "ObjectBase.h"
#include <list>

/// <summary>
/// 衝突マネージャ
/// </summary>
class CollisionManager {
public:
    /// <summary>
    /// リセット
    /// </summary>
    void Reset();

    /// <summary>
    /// コライダーのリストに登録
    /// </summary>
    void AddCollider(ObjectBase* characterBase);

    /// <summary>
    /// コライダー2つの衝突判定と応答
    /// </summary>
    void CheckColliderPair(ObjectBase* characterA, ObjectBase* characterB);

    /// <summary>
    /// すべての当たり判定チェック
    /// </summary>
    void CheckAllCollisions();

private:
    /// ===コライダー=== ///
    std::list<ObjectBase*> objects_;
};
