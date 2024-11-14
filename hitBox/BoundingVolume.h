/*********************************************************************
 * \file   BoundingVolume.h
 * \brief
 *
 * \author Harukichimaru
 * \date   November 2024
 * \note
 *********************************************************************/
#pragma once
#include <algorithm>
#include "Vector3.h"  // Vector3を仮定

 ///=============================================================================
 ///						 クラスの定義
class BoundingVolume {
	///--------------------------------------------------------------
	///						 メンバ関数
public:
	
	/**----------------------------------------------------------------------------
	 * \brief  BoundingVolume 
	 * \note   
	 */
	BoundingVolume() = default;


	/**----------------------------------------------------------------------------
	 * \brief  BoundingVolume コンストラクタ（AABBの最小・最大座標で初期化）
	 * \param  min 
	 * \param  max
	 * \note   
	 */
	BoundingVolume(const Vector3& min, const Vector3& max) : min(min), max(max) {}

	/**----------------------------------------------------------------------------
	 * \brief  Intersects AABB同士の交差判定
	 * \param  other
	 * \return 
	 * \note   
	 */
	bool Intersects(const BoundingVolume& other) const {
		return ( min.x <= other.max.x && max.x >= other.min.x ) &&
			( min.y <= other.max.y && max.y >= other.min.y ) &&
			( min.z <= other.max.z && max.z >= other.min.z );
	}

	/**----------------------------------------------------------------------------
	 * \brief  Merge 二つのBoundingVolumeを統合したAABBを計算する
	 * \param  a
	 * \param  b
	 * \return 
	 * \note   
	 */
	static BoundingVolume Merge(const BoundingVolume& a, const BoundingVolume& b) {
		Vector3 mergedMin(std::min(a.min.x, b.min.x),
			std::min(a.min.y, b.min.y),
			std::min(a.min.z, b.min.z));
		Vector3 mergedMax(std::max(a.max.x, b.max.x),
			std::max(a.max.y, b.max.y),
			std::max(a.max.z, b.max.z));
		return BoundingVolume(mergedMin, mergedMax);
	}

	/**----------------------------------------------------------------------------
	 * \brief  GetCenter AABBの中央座標を取得（BVHの分割基準に利用）
	 * \return 
	 * \note   
	 */
	Vector3 GetCenter() const {
		return ( min + max ) * 0.5f;
	}

	///--------------------------------------------------------------
	///						 メンバ変数
private:
	Vector3 min;  // バウンディングボリュームの最小座標
	Vector3 max;  // バウンディングボリュームの最大座標
};

