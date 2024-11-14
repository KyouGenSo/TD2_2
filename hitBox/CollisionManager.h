/*********************************************************************
 * \file   CollisionManager.h
 * \brief  
 * 
 * \author Harukichimaru
 * \date   November 2024
 * \note   
 *********************************************************************/
#pragma once
#include <vector>
#include <utility>
#include "BVHNode.h"
#include "Collider.h"
#include "ObjectBase.h"

 ///=============================================================================
 ///						 クラスの定義
class CollisionManager {
	///--------------------------------------------------------------
	///     					 メンバ関数
public:

    /**----------------------------------------------------------------------------
     * \brief  CollisionManager 
     * \note   
     */
    CollisionManager() : root(nullptr) {}

    /**----------------------------------------------------------------------------
     * \brief  BuildBVH BVHの構築
     * \param  objects
     * \note   
     */
    void BuildBVH(std::vector<Collider*>& objects);

    /**----------------------------------------------------------------------------
     * \brief  ProcessCollisions ブロードフェーズとナローフェーズの実行
     * \note   
     */
    void ProcessCollisions();

    ///--------------------------------------------------------------
	///						 メンバ変数
private:
    BVHNode* root;  // BVHのルートノード
};


