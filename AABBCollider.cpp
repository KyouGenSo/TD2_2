#include "AABBCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"

bool AABBCollider::Intersects(const Collider& other) const {
    // AABB同士の判定
    if(other.GetType() == ColliderType::AABB) {
        const AABBCollider& aabb = static_cast<const AABBCollider&>( other );
        return ( min_.x <= aabb.max_.x && max_.x >= aabb.min_.x ) &&
            ( min_.y <= aabb.max_.y && max_.y >= aabb.min_.y ) &&
            ( min_.z <= aabb.max_.z && max_.z >= aabb.min_.z );
    }
    // Sphereとの判定
    if(other.GetType() == ColliderType::Sphere) {
        const SphereCollider& sphere = static_cast<const SphereCollider&>( other );
        float distance = 0.0f;
        if(sphere.GetCenter().x < min_.x) {
            distance += ( sphere.GetCenter().x - min_.x ) * ( sphere.GetCenter().x - min_.x );
        } else if(sphere.GetCenter().x > max_.x) {
            distance += ( sphere.GetCenter().x - max_.x ) * ( sphere.GetCenter().x - max_.x );
        }
        if(sphere.GetCenter().y < min_.y) {
            distance += ( sphere.GetCenter().y - min_.y ) * ( sphere.GetCenter().y - min_.y );
        } else if(sphere.GetCenter().y > max_.y) {
            distance += ( sphere.GetCenter().y - max_.y ) * ( sphere.GetCenter().y - max_.y );
        }
        if(sphere.GetCenter().z < min_.z) {
            distance += ( sphere.GetCenter().z - min_.z ) * ( sphere.GetCenter().z - min_.z );
        } else if(sphere.GetCenter().z > max_.z) {
            distance += ( sphere.GetCenter().z - max_.z ) * ( sphere.GetCenter().z - max_.z );
        }
        return distance <= ( sphere.GetRadius() * sphere.GetRadius() );
    }
    // カプセルとの判定
    if(other.GetType() == ColliderType::Capsule) {
        const CapsuleCollider& capsule = static_cast<const CapsuleCollider&>( other );
        const Vector3& start = capsule.GetStart();
        //const Vector3& end = capsule.GetEnd();
        float radius = capsule.GetRadius();

        // AABBの各辺とカプセルの線分の最近接点を求める
        Vector3 closestPoint = start;
        if(start.x < min_.x) closestPoint.x = min_.x;
        else if(start.x > max_.x) closestPoint.x = max_.x;
        if(start.y < min_.y) closestPoint.y = min_.y;
        else if(start.y > max_.y) closestPoint.y = max_.y;
        if(start.z < min_.z) closestPoint.z = min_.z;
        else if(start.z > max_.z) closestPoint.z = max_.z;

        float distance = ( ( closestPoint.x - start.x ) * ( closestPoint.x - start.x ) ) +
            ( ( closestPoint.y - start.y ) * ( closestPoint.y - start.y ) ) +
            ( ( closestPoint.z - start.z ) * ( closestPoint.z - start.z ) );
        return distance <= ( radius * radius );
    }

    return false;
}