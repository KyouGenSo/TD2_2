/*********************************************************************
 * \file   CollisionManager.cpp
 * \brief
 * 
 * \author Harukichimaru
 * \date   November 2024
 * \note   
 *********************************************************************/
#include "CollisionManager.h"

///=============================================================================
///						BVHの構築
void CollisionManager::BuildBVH(std::vector<Collider*>& objects) {
    root = new BVHNode();
    root->Build(objects);
}

///=============================================================================
///					    ブロードフェーズとナローフェーズの実行
void CollisionManager::ProcessCollisions() {
    std::vector<std::pair<Collider*, Collider*>> potentialCollisions;
    root->GetPotentialCollisions(potentialCollisions);

    // ナローフェーズでの詳細な衝突判定
    for(auto& pair : potentialCollisions) {
        if(pair.first->CollidesWith(*pair.second)) {
            // 各オブジェクトのOnCollisionを呼び出す
            static_cast<ObjectBase*>( pair.first )->OnCollision(pair.second);
            static_cast<ObjectBase*>( pair.second )->OnCollision(pair.first);
        }
    }
}
