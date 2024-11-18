/*********************************************************************
 * \file   CollisionManager.cpp
 * \brief  
 * 
 * \author Harukichimaru
 * \date   November 2024
 * \note   
 *********************************************************************/
#include "CollisionManager.h"

///-------------------------------------------///
/// リセット
///-------------------------------------------///
void CollisionManager::Reset() {
    // リストを空っぽにする
    Objects_.clear();
}

///-------------------------------------------///
/// コライダーのリストに登録
///-------------------------------------------///
void CollisionManager::AddCollider(ObjectBase* ObjectBase) {
    // コライダーをリストに追加
    Objects_.push_back(ObjectBase);
}

///-------------------------------------------///
/// コライダー2つの衝突判定と応答
///-------------------------------------------///
void CollisionManager::CheckColliderPair(ObjectBase* characterA, ObjectBase* characterB) {
    if (characterA->GetCollider()->Intersects(*characterB->GetCollider())) {
        // コライダーAの衝突コールバックを呼び出す
        characterA->OnCollision(characterB);

        // コライダーBの衝突コールバックを呼び出す
        characterB->OnCollision(characterA);
    }
}

///-------------------------------------------///
/// すべての当たり判定チェック
///-------------------------------------------///
void CollisionManager::CheckAllCollisions() {
    // リスト内のペアを総当り
    std::list<ObjectBase*>::iterator itrA = Objects_.begin();

    for (; itrA != Objects_.end(); ++itrA) {
        ObjectBase* characterA = *itrA;

        // イテレーターBはイテレータAの次の要素から回す(重複判定を回避)
        std::list<ObjectBase*>::iterator itrB = itrA;
        itrB++;

        for (; itrB != Objects_.end(); ++itrB) {
            ObjectBase* characterB = *itrB;

            // ペアの当たり判定
            CheckColliderPair(characterA, characterB);
        }
    }
}
