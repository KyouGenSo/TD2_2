/*********************************************************************
 * \file   SphereCollider.h
 * \brief  
 * 
 * \author Harukichimaru
 * \date   November 2024
 * \note   
 *********************************************************************/
#pragma once
#include "Collider.h"

///=============================================================================
///					クラスの定義
class SphereCollider : public Collider {
    ///--------------------------------------------------------------
	///						 メンバ関数
public:
    /**----------------------------------------------------------------------------
     * \brief  SphereCollider  
     * \note   
     */
    SphereCollider() { type = ColliderType::Sphere; }

    /**----------------------------------------------------------------------------
     * \brief  GetBoundingVolume 
     * \return 
     * \note   
     */
    BoundingVolume GetBoundingVolume() const override;

    /**----------------------------------------------------------------------------
     * \brief  CollidesWith 
     * \param  other
     * \return 
     * \note   
     */
    bool CollidesWith(const Collider& other) const override;

    ///--------------------------------------------------------------
	///						 メンバ変数
private:
public:
    Vector3 center;
    float radius;
};


