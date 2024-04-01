#pragma once

#include "Vec2.h"

enum class BodyType
{
    DYNAMIC,
    STATIC,
    NONE
};


/**
 * @brief Represents a 2D body with position, velocity, and mass.
 * @note A body is disabled if its mass is negative.
 */
class Body {
public:
    Math::Vec2F Position = Math::Vec2F::Zero();
    Math::Vec2F Velocity = Math::Vec2F::Zero();
    float Mass = -1.f; // Body is disabled if mass is negative
    BodyType Type = BodyType::DYNAMIC;
private:
    Math::Vec2F _force = Math::Vec2F::Zero(); // Total force acting on the body
public:
    /**
     * @brief Default constructor for Body.
     */
    constexpr Body() noexcept = default;

    /**
     * @brief Constructor for Body with specified position, velocity, and mass.
     * @param position The initial position of the body.
     * @param velocity The initial velocity of the body.
     * @param mass The mass of the body.
     */
    constexpr explicit Body(Math::Vec2F position, Math::Vec2F velocity, float mass) noexcept
            : Position(position), Velocity(velocity), Mass(mass) {}

    /**
     * @brief Apply a force to the body.
     * @param force The force to be applied.
     */
    void ApplyForce(const Math::Vec2F &force) noexcept;

    /**
     * @brief Check if the body is enabled (mass is non-negative).
     * @return true if the body is enabled, false otherwise.
     */
    [[nodiscard]] constexpr bool IsEnabled() const noexcept { return Mass >= 0; }

    /**
     * @brief Enable the body by setting its mass to a positive value.
     */
    constexpr void Enable() noexcept { Mass = 1.f; }

    /**
     * @brief Disable the body by setting its mass to a negative value.
     */
    constexpr void Disable() noexcept { Mass = -1.f; }

    /**
     * @brief Get the total force acting on the body.
     * @return The total force acting on the body.
     */
    [[nodiscard]] constexpr Math::Vec2F GetForce() const { return _force; }

    /**
     * @brief Reset the total force acting on the body to zero.
     */
    constexpr void ResetForce() { _force = Math::Vec2F::Zero(); }
};