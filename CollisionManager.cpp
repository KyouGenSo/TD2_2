#include "CollisionManager.h"

void CollisionManager::Reset() {
    objects_.clear();
}

void CollisionManager::AddCollider(ObjectBase* object) {
    objects_.push_back(object);
}

void CollisionManager::CheckColliderPair(ObjectBase* objectA, ObjectBase* objectB) {
    if (objectA->GetCollider()->Intersects(*objectB->GetCollider())) {
        objectA->OnCollision(objectB);
        objectB->OnCollision(objectA);
    }
}

void CollisionManager::CheckAllCollisions() {
    for (auto itA = objects_.begin(); itA != objects_.end(); ++itA) {
        for (auto itB = std::next(itA); itB != objects_.end(); ++itB) {
            CheckColliderPair(*itA, *itB);
        }
    }
}
