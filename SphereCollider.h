/*********************************************************************
 * \file   SphereCollider.h
 * \brief  
 * 
 * \author Harukichimaru
 * \date   November 2024
 * \note   
 *********************************************************************/
#pragma once
#include "Vector3.h"
#include <memory>
#include "Collider.h"

 ///=============================================================================
 ///						クラスの定義
class SphereCollider : public Collider {
    ///--------------------------------------------------------------
	///						 メンバ関数
public:
    /**----------------------------------------------------------------------------
     * \brief  SphereCollider 
     * \param  center
     * \param  radius
     * \note   
     */
    SphereCollider(const Vector3& center, float radius) : center_(center), radius_(radius) {}

    /**----------------------------------------------------------------------------
     * \brief  GetType 
     * \return 
     * \note   
     */
    ColliderType GetType() const override { return ColliderType::Sphere; }

    /**----------------------------------------------------------------------------
     * \brief  Intersects 
     * \param  other
     * \return 
     * \note   
     */
    bool Intersects(const Collider& other) const override;

    /**----------------------------------------------------------------------------
     * \brief  GetCenter 
     * \return 
     * \note   
     */
    const Vector3& GetCenter() const { return center_; }

    /**----------------------------------------------------------------------------
     * \brief  GetRadius 
     * \return 
     * \note   
     */
    float GetRadius() const { return radius_; }

    ///--------------------------------------------------------------
	///						 メンバ変数
private:
    //========================================
	// 中心座標
    Vector3 center_;

    //========================================
	// 半径
    float radius_;
};

