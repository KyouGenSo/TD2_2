/*********************************************************************
 * \file   Collider.h
 * \brief  
 * 
 * \author Harukichimaru
 * \date   November 2024
 * \note   
 *********************************************************************/
#pragma once
#include "Vector3.h"
#include <memory>

enum class ColliderType {
    AABB,
    Sphere,
    Capsule
};

class Collider {
public:
    virtual ~Collider() = default;
    virtual ColliderType GetType() const = 0;
    virtual bool Intersects(const Collider& other) const = 0;
};
