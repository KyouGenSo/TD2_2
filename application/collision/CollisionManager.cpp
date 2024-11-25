#include "CollisionManager.h"
#include "ModelManager.h"
#include "Object3dBasic.h"
#include "Draw2D.h"
#include "Camera.h" // Add this include to resolve the incomplete type error

void CollisionManager::Initialize() {
}

void CollisionManager::Update() {
}

void CollisionManager::Draw() {
	// 登録されているObjectBaseの描画
	std::list<ObjectBase*>::iterator itr = Objects_.begin();
	for(; itr != Objects_.end(); ++itr) {
		// 始点と終点を取得
		Vector3 start = ( *itr )->GetCollider()->GetStart();
		Vector3 end = ( *itr )->GetCollider()->GetEnd();
		//半径の取得
		float radius = ( *itr )->GetCollider()->GetRadius();
		Vector4 color = ( *itr )->GetCollider()->GetColor();


		//ワイヤーフレームの描画
		Matrix4x4 viewProjectionMatrix = Object3dBasic::GetInstance()->GetCamera()->GetViewProjectionMatrix();
		Draw2D::GetInstance()->DrawCapsule(radius, start, end, color, viewProjectionMatrix);

		//白色に変更
		( *itr )->GetCollider()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
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
	if(characterA->GetCollider()->Intersects(*characterB->GetCollider())) {
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

	for(; itrA != Objects_.end(); ++itrA) {
		ObjectBase* characterA = *itrA;

		// イテレーターBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<ObjectBase*>::iterator itrB = itrA;
		itrB++;

		for(; itrB != Objects_.end(); ++itrB) {
			ObjectBase* characterB = *itrB;

			// ペアの当たり判定
			CheckColliderPair(characterA, characterB);
		}
	}
}
