/*********************************************************************
 * \file   AABBCollider.h
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
class AABBCollider : public Collider {
	///--------------------------------------------------------------
	///						 メンバ関数
public:
	/**----------------------------------------------------------------------------
	 * \brief  AABBCollider
	 * \param  min
	 * \param  max
	 * \note
	 */
	AABBCollider(const Vector3& min, const Vector3& max) : min_(min), max_(max) {}

	/**----------------------------------------------------------------------------
	 * \brief  GetType
	 * \return
	 * \note
	 */
	ColliderType GetType() const override { return ColliderType::AABB; }

	/**----------------------------------------------------------------------------
	 * \brief  Intersects
	 * \param  other
	 * \return
	 * \note
	 */
	bool Intersects(const Collider& other) const override;

	/**----------------------------------------------------------------------------
	 * \brief  GetMin
	 * \return
	 * \note
	 */
	const Vector3& GetMin() const { return min_; }

	/**----------------------------------------------------------------------------
	 * \brief  GetMax
	 * \return
	 * \note
	 */
	const Vector3& GetMax() const { return max_; }

	///--------------------------------------------------------------
	///						 メンバ変数
private:
	//========================================
	// 最小値
	Vector3 min_;

	//========================================
	// 最大値
	Vector3 max_;
};
