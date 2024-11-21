/*********************************************************************
 * \file   CollisionManager.cpp
 * \brief  
 * 
 * \author Harukichimaru
 * \date   November 2024
 * \note   
 *********************************************************************/
#include "CollisionManager.h"
#include "ModelManager.h"

void CollisionManager::Initialize() {
    // SRT
     Transform transform_;

    // プレイヤーモデルの読み込みと設定
    ModelManager::GetInstance()->LoadModel("capsuleHead.obj");
    object3dStart_ = std::make_unique<Object3d>();
    object3dStart_->Initialize();
    object3dStart_->SetModel("capsuleHead.obj");

    object3dEnd_ = std::make_unique<Object3d>();
    object3dEnd_->Initialize();
    object3dEnd_->SetModel("capsuleHead.obj");

    // スケール、回転、位置の初期設定
    transform_.scale = { 1.0f, 1.0f, 1.0f };
    transform_.rotate = { 0.0f, 0.0f, 0.0f };
    transform_.translate = { 0.0f, 0.0f, 0.0f };

    // モデルの更新
    object3dStart_->SetScale(transform_.scale);
    object3dStart_->SetRotate(transform_.rotate);
    object3dStart_->SetTranslate(transform_.translate);

    object3dEnd_->SetScale(transform_.scale);
    object3dEnd_->SetRotate(transform_.rotate);
    object3dEnd_->SetTranslate(transform_.translate);
}

void CollisionManager::Update() {
	// モデルの更新
	object3dStart_->Update();
	object3dEnd_->Update(); 
}

void CollisionManager::Draw() {
    // 登録されているObjectBaseの描画
    std::list<ObjectBase*>::iterator itr = Objects_.begin();
    for (; itr != Objects_.end(); ++itr) {
        // 始点と終点を取得
        Vector3 start = (*itr)->GetCollider()->GetStart();
        Vector3 end = (*itr)->GetCollider()->GetEnd();
        //半径の取得
		float radius = ( *itr )->GetCollider()->GetRadius();
        
        // 始点にオブジェクトを描画
        object3dStart_->SetTranslate(start);
		object3dStart_->SetScale({ radius, radius, radius });
        object3dStart_->Draw();
        
        // 終点にオブジェクトを描画
        object3dEnd_->SetTranslate(end);
        object3dEnd_->SetScale({ radius, radius, radius });
        object3dEnd_->Draw();
    }
}

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
