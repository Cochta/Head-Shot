#pragma once
#pragma once

#include "Shape.h"
#include "Refs.h"

#include <variant>

/**
 * @file Collider.h
 * @brief This file defines the `Shape`, `Collider`, `ColliderPair` and `ColliderPairHash` structures
 */



 /**
  * @class Collider
  * @brief Represents a collider in a physics simulation.
  */
class Collider
{
public:
	std::variant<Math::CircleF, Math::RectangleF, Math::PolygonF> Shape{
			Math::CircleF(Math::Vec2F::Zero(), 1) }; /**< The shape associated with the collider. */

	BodyRef BodyRef; /**< Reference to the body associated with the collider. */

	Math::Vec2F BodyPosition = Math::Vec2F::Zero();/**< Position of the body associated to the collider. */

	float Restitution = 1.f; /**< Bounciness/Restition of the collider. */

	bool IsTrigger = false; /**< Flag indicating if the collider is a trigger (non-physical). */
	bool IsAttached = false; /**< Flag indicating if the collider is attached to a body. */

	[[nodiscard]] Math::RectangleF GetBounds() const noexcept;
};

/**
 * @struct ColliderPair
 * @brief Represents a pair of colliders involved in a collision.
 */
struct ColliderRefPair
{
	ColliderRef ColRefA; /**< The first colliderRef in the pair. */
	ColliderRef ColRefB; /**< The second colliderRef in the pair. */

	bool operator==(const ColliderRefPair& other) const;

};

/**
 * @class ColliderPairHash
 * @brief Custom hash function for collider pairs.
 */
struct ColliderRefPairHash
{
	/**
	 * @brief Calculate a hash value for a collider pair.
	 * @param pair The collider pair to hash.
	 * @return The hash value based on the individual hash values of the colliders.
	 */
	std::size_t operator()(const ColliderRefPair& pair) const;
};