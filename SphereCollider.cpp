#include "SphereCollider.h"
#include "AABBCollider.h"
#include "CapsuleCollider.h"

bool SphereCollider::Intersects(const Collider& other) const {
    if(other.GetType() == ColliderType::Sphere) {
        const SphereCollider& sphere = static_cast<const SphereCollider&>( other );
        float distance = ( center_.x - sphere.center_.x ) * ( center_.x - sphere.center_.x ) +
            ( center_.y - sphere.center_.y ) * ( center_.y - sphere.center_.y ) +
            ( center_.z - sphere.center_.z ) * ( center_.z - sphere.center_.z );
        return distance < ( ( radius_ + sphere.radius_ ) * ( radius_ + sphere.radius_ ) );
    }
    if(other.GetType() == ColliderType::AABB) {
        return other.Intersects(*this);
    }
    if(other.GetType() == ColliderType::Capsule) {
        const CapsuleCollider& capsule = static_cast<const CapsuleCollider&>( other );
        const Vector3& start = capsule.GetStart();
        const Vector3& end = capsule.GetEnd();
        float radius = capsule.GetRadius();

        // 球とカプセルの線分の最近接点を求める
        Vector3 closestPoint = start;
        if(center_.x < start.x) closestPoint.x = start.x;
        else if(center_.x > end.x) closestPoint.x = end.x;
        if(center_.y < start.y) closestPoint.y = start.y;
        else if(center_.y > end.y) closestPoint.y = end.y;
        if(center_.z < start.z) closestPoint.z = start.z;
        else if(center_.z > end.z) closestPoint.z = end.z;

        float distance = ( ( closestPoint.x - center_.x ) * ( closestPoint.x - center_.x ) ) +
            ( ( closestPoint.y - center_.y ) * ( closestPoint.y - center_.y ) ) +
            ( ( closestPoint.z - center_.z ) * ( closestPoint.z - center_.z ) );
        return distance <= ( ( radius + this->radius_ ) * ( radius + this->radius_ ) );
    }

    return false;
}