/*********************************************************************
 * \file   CollisionManager.h
 * \brief  
 * 
 * \author Harukichimaru
 * \date   November 2024
 * \note   
 *********************************************************************/
#pragma once
#include "ObjectBase.h"
#include "Collider.h"
#include <list>

 ///=============================================================================
 ///						クラス定義　
class CollisionManager {
    ///--------------------------------------------------------------
    ///						 メンバ変数
public:
    /**----------------------------------------------------------------------------
	 * \brief  Reset リセット
     * \note   
     */
    void Reset();

    /**----------------------------------------------------------------------------
	 * \brief  AddCollider 当たり判定を追加
	 * \param  characterBase 追加する当たり判定
     * \note   
     */
    void AddCollider(ObjectBase* characterBase);

    /**----------------------------------------------------------------------------
	 * \brief  CheckColliderPair 当たり判定同士をチェック
     * \param  characterA
     * \param  characterB
     * \note   
     */
    void CheckColliderPair(ObjectBase* characterA, ObjectBase* characterB);

    /**----------------------------------------------------------------------------
	 * \brief  CheckAllCollisions すべての当たり判定をチェック
     * \note   
     */
    void CheckAllCollisions();

    ///--------------------------------------------------------------
	///						 メンバ変数
private:
    /// ===コライダー=== ///
    std::list<ObjectBase*> Objects_;
};
