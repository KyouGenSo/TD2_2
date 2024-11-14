#include "CapsuleCollider.h"
#include "AABBCollider.h"
#include "SphereCollider.h"

bool CapsuleCollider::Intersects(const Collider& other) const {
	if(other.GetType() == ColliderType::Capsule) {
		const CapsuleCollider& capsule = static_cast<const CapsuleCollider&>( other );
		const Vector3& start1 = this->GetStart();
		//const Vector3& end1 = this->GetEnd();
		const Vector3& start2 = capsule.GetStart();
		const Vector3& end2 = capsule.GetEnd();
		float radiusSum = this->GetRadius() + capsule.GetRadius();

		// カプセル同士の線分の最近接点を求める
		Vector3 closestPoint1 = start1;
		Vector3 closestPoint2 = start2;
		if(start1.x < start2.x) closestPoint1.x = start2.x;
		else if(start1.x > end2.x) closestPoint1.x = end2.x;
		if(start1.y < start2.y) closestPoint1.y = start2.y;
		else if(start1.y > end2.y) closestPoint1.y = end2.y;
		if(start1.z < start2.z) closestPoint1.z = start2.z;
		else if(start1.z > end2.z) closestPoint1.z = end2.z;

		float distance = ( ( closestPoint1.x - closestPoint2.x ) * ( closestPoint1.x - closestPoint2.x ) ) +
			( ( closestPoint1.y - closestPoint2.y ) * ( closestPoint1.y - closestPoint2.y ) ) +
			( ( closestPoint1.z - closestPoint2.z ) * ( closestPoint1.z - closestPoint2.z ) );
		return distance <= ( radiusSum * radiusSum );
	}
	if(other.GetType() == ColliderType::AABB) {
		return other.Intersects(*this);
	}
	if(other.GetType() == ColliderType::Sphere) {
		return other.Intersects(*this);
	}

	return false;
}