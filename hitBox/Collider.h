/*********************************************************************
 * \file   Collider.h
 * \brief  
 * 
 * \author Harukichimaru
 * \date   November 2024
 * \note   
 *********************************************************************/
#pragma once
#include "BoundingVolume.h"

 ///=============================================================================
 ///						クラスの定義
class Collider {
    ///--------------------------------------------------------------
    ///						 メンバ関数
public:

	/**
	 * \brief コライダーの形状
	 */
	enum class ColliderType { Sphere, AABB, Capsule };
	ColliderType type;

    /**----------------------------------------------------------------------------
     * \brief  ~Collider 
     * \note   
     */
    virtual ~Collider() = default;

	/**----------------------------------------------------------------------------
	 * \brief  GetBoundingVolume バウンディングボリュームの取得
	 * \return
	 * \note
	 */
    virtual BoundingVolume GetBoundingVolume() const = 0; // バウンディングボリュームの取得

   
	/**----------------------------------------------------------------------------
	* \brief  CollidesWith ナローフェーズの衝突判定
	* \param  other
	* \return
	* \note
	*/
    virtual bool CollidesWith(const Collider& other) const = 0; // ナローフェーズの衝突判定
};



