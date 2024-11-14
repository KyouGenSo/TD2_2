#pragma once
#include "Model.h"
#include "Vector3.h"
#include "Object3d.h"
#include <memory>
#include "Input.h"
#include "Collider.h"

class ObjectBase {
public:
    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="position">初期位置</param>
    virtual void Init(const Vector3& position);

    /// <summary>
    /// 更新
    /// </summary>
    virtual void Update();

    /// <summary>
    /// 衝突処理イベント
    /// </summary>
    /**----------------------------------------------------------------------------
     * \brief  OnCollisionEnter 
     * \param  objectBase
     * \note   
     */
    virtual void OnCollisionEnter(ObjectBase* objectBase) = 0;

    /**----------------------------------------------------------------------------
     * \brief  OnCollisionStay 
     * \param  objectBase
     * \note   
     */
    virtual void OnCollisionStay(ObjectBase* objectBase) = 0;

    /**----------------------------------------------------------------------------
     * \brief  OnCollisionExit 
     * \param  objectBase
     * \note   
     */
    virtual void OnCollisionExit(ObjectBase* objectBase) = 0;


    /// <summary>
    /// コライダーのゲッター
    /// </summary>
    /// <returns>コライダー</returns>
    std::unique_ptr<Collider>& GetCollider() { return collider_; }

protected:
    // コライダー
    std::unique_ptr<Collider> collider_;
};