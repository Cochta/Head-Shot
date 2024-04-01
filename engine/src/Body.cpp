#include "Body.h"

void Body::ApplyForce(const Math::Vec2F &force) noexcept
{
    _force += force;
}