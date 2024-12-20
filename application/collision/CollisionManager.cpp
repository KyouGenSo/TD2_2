#include "CollisionManager.h"
#include "ModelManager.h"
#include "Object3dBasic.h"
#include "Draw2D.h"
#include "Camera.h"
#include "ImGuiManager.h"

void CollisionManager::Initialize() {
}

void CollisionManager::Update() {
}

void CollisionManager::Draw() {
	// Capsuleワイヤーの描画有無
	if (isDrawCapsule_) {
		// 登録されているObjectBaseの描画
		std::list<ObjectBase*>::iterator itr = Objects_.begin();
		for (; itr != Objects_.end(); ++itr) {
			// 始点と終点を取得
			Vector3 start = (*itr)->GetCollider()->GetStart();
			Vector3 end = (*itr)->GetCollider()->GetEnd();
			//半径の取得
			float radius = (*itr)->GetCollider()->GetRadius();
			Vector4 color = (*itr)->GetCollider()->GetColor();


			//ワイヤーフレームの描画
			Matrix4x4 viewProjectionMatrix = Object3dBasic::GetInstance()->GetCamera()->GetViewProjectionMatrix();
			Draw2D::GetInstance()->DrawCapsule(radius, start, end, color, viewProjectionMatrix);

			//白色に変更
			(*itr)->GetCollider()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

///=============================================================================
///						Imguiの描画
void CollisionManager::DrawImGui() {
	ImGui::Begin("Collision MGR");
	//ワイヤーフレームの描画
	ImGui::Checkbox("DrawCapsule", &isDrawCapsule_);
	ImGui::End();
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
		characterA->OnCollision(characterB); // 衝突したペアにのみ処理
		characterB->OnCollision(characterA);
	}
}

///-------------------------------------------///
/// すべての当たり判定チェック
///-------------------------------------------///
void CollisionManager::CheckAllCollisions() {
	for (auto itrA = Objects_.begin(); itrA != Objects_.end(); ++itrA) {
		for (auto itrB = std::next(itrA); itrB != Objects_.end(); ++itrB) {
			CheckColliderPair(*itrA, *itrB);
		}
	}
}
