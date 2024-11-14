/*********************************************************************
 * \file   BVHNode.h
 * \brief  
 * 
 * \author Harukichimaru
 * \date   November 2024
 * \note   
 *********************************************************************/
#pragma once
#include <vector>
#include <utility>
#include "BoundingVolume.h"
#include "Collider.h"

 ///=============================================================================
 ///						 クラスの定義
class BVHNode {
	///--------------------------------------------------------------
	///	    				 メンバ関数
public:

    BVHNode() : left(nullptr), right(nullptr), object(nullptr) {}

    // BVH構造を再帰的に構築するメソッド
    void Build(std::vector<Collider*>& objects);

    // 衝突候補を収集するメソッド
    void GetPotentialCollisions(std::vector<std::pair<Collider*, Collider*>>& pairs) const;

    ///--------------------------------------------------------------
	///						 メンバ変数
private:
    BoundingVolume volume;     // ノードのバウンディングボリューム
    BVHNode* left;             // 左の子ノード
    BVHNode* right;            // 右の子ノード
    Collider* object;          // ノードに含まれるオブジェクト（リーフノードのみ）
};


