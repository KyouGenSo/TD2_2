/*********************************************************************
 * \file   CapsuleCollider.h
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
 ///						
class CapsuleCollider : public Collider {
    ///--------------------------------------------------------------
	///						 メンバ関数
public:
    /**----------------------------------------------------------------------------
     * \brief  CapsuleCollider 
     * \param  start
     * \param  end
     * \param  radius
     * \note   
     */
    CapsuleCollider(const Vector3& start, const Vector3& end, float radius) : start_(start), end_(end), radius_(radius) {}

    /**----------------------------------------------------------------------------
     * \brief  GetType 
     * \return 
     * \note   
     */
    ColliderType GetType() const override { return ColliderType::Capsule; }

    /**----------------------------------------------------------------------------
     * \brief  Intersects 
     * \param  other
     * \return 
     * \note   
     */
    bool Intersects(const Collider& other) const override;

    /**----------------------------------------------------------------------------
     * \brief  GetStart 
     * \return 
     * \note   
     */
    const Vector3& GetStart() const { return start_; }

    /**----------------------------------------------------------------------------
     * \brief  GetEnd 
     * \return 
     * \note   
     */
    const Vector3& GetEnd() const { return end_; }

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
	// 始点
    Vector3 start_;

    //========================================
	// 終点
    Vector3 end_;

    //========================================
	// 半径
    float radius_;
};

