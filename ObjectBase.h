/*********************************************************************
 * \file   ObjectBase.h
 * \brief
 *
 * \author Harukichimaru
 * \date   November 2024
 * \note
 *********************************************************************/
#pragma once

#include "hitBox/Collider.h"

 ///=============================================================================
 /// 						 クラスの定義
class ObjectBase {
	///--------------------------------------------------------------
	///						 メンバ関数
public:

	/**----------------------------------------------------------------------------
	 * \brief  OnCollisionEnter 触れた時
	 * \param  other
	 * \note   
	 */
	virtual void OnCollisionEnter(const ObjectBase& other) = 0;

	/**----------------------------------------------------------------------------
	 * \brief  OnCollisionStay 触れ続けている時
	 * \param  other
	 * \note   
	 */
	virtual void OnCollisionStay(const ObjectBase& other) = 0;

	/**----------------------------------------------------------------------------
	 * \brief  OnCollisionExit 離れた時
	 * \param  other
	 * \note   
	 */
	virtual void OnCollisionExit(const ObjectBase& other) = 0;

	/**----------------------------------------------------------------------------
	 * \brief  GetCollider コライダーの取得
	 * \return 
	 * \note   
	 */
	virtual const Collider& GetCollider() const = 0;

	///--------------------------------------------------------------
	///						 メンバ変数
private:
	//========================================
	// コライダー
	Collider* collider;
};

