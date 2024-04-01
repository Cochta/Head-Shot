#include "Contact.h"

void Contact::Resolve()
{
	switch (CollidingBodies[0].collider->Shape.index())
	{
	case static_cast<int>(Math::ShapeType::Circle):
		switch (CollidingBodies[1].collider->Shape.index())
		{
		case static_cast<int>(Math::ShapeType::Circle):
		{
			const Math::CircleF& circle0 = std::get<Math::CircleF>(CollidingBodies[0].collider->Shape);
			const Math::CircleF& circle1 = std::get<Math::CircleF>(CollidingBodies[1].collider->Shape);

			const auto delta = CollidingBodies[0].body->Position + circle0.Center() - CollidingBodies[1].body->Position - circle1.Center();

			if (delta.Length() > 0.f)
			{
				Normal = delta.Normalized();
			}
			else
			{
				Normal = Math::Vec2F::Up();
			}
			Penetration = std::get<Math::CircleF>(CollidingBodies[0].collider->Shape).Radius() +
				std::get<Math::CircleF>(CollidingBodies[1].collider->Shape).Radius() - delta.Length();
		}
		break;
		case static_cast<int>(Math::ShapeType::Rectangle):
		{
			const Math::CircleF& circle = std::get<Math::CircleF>(CollidingBodies[0].collider->Shape);
			const Math::RectangleF& rectangle = std::get<Math::RectangleF>(CollidingBodies[1].collider->Shape);

			const Math::Vec2F closest(
				Math::Clamp(CollidingBodies[0].body->Position.X + circle.Center().X, rectangle.MinBound().X + CollidingBodies[1].body->Position.X, rectangle.MaxBound().X + CollidingBodies[1].body->Position.X),
				Math::Clamp(CollidingBodies[0].body->Position.Y + circle.Center().Y, rectangle.MinBound().Y + CollidingBodies[1].body->Position.Y, rectangle.MaxBound().Y + CollidingBodies[1].body->Position.Y));

			const Math::Vec2F delta = CollidingBodies[0].body->Position + circle.Center() - closest;

			const float distance = delta.Length();

			Penetration = circle.Radius() - distance;

			if (distance > 0.f)
			{
				Normal = delta.Normalized();
			}
			else
			{
				Normal = Math::Vec2F::Up();
			}
			
		}
		break;
		}
		break;
	case static_cast<int>(Math::ShapeType::Rectangle):
		switch (CollidingBodies[1].collider->Shape.index())
		{
		case static_cast<int>(Math::ShapeType::Circle):
		{
			std::swap(CollidingBodies[0], CollidingBodies[1]);
			Resolve();
		}
		break;
		case static_cast<int>(Math::ShapeType::Rectangle):
		{
			const Math::RectangleF& rect0 = std::get<Math::RectangleF>(CollidingBodies[0].collider->Shape);
			const Math::RectangleF& rect1 = std::get<Math::RectangleF>(CollidingBodies[1].collider->Shape);

			const Math::Vec2F delta = CollidingBodies[0].body->Position + rect0.Center() - CollidingBodies[1].body->Position - rect1.Center();

			const Math::Vec2F penetration(
				std::get<Math::RectangleF>(CollidingBodies[0].collider->Shape).HalfSize() +
				std::get<Math::RectangleF>(CollidingBodies[1].collider->Shape).HalfSize() -
				Math::Vec2F(std::abs(delta.X), std::abs(delta.Y))
			);

			if (penetration.X < penetration.Y)
			{
				Penetration = penetration.X;
				Normal = (delta.X > 0) ? Math::Vec2F(1.0f, 0.0f) : Math::Vec2F(-1.0f, 0.0f);
			}
			else
			{
				Penetration = penetration.Y;
				Normal = (delta.Y > 0) ? Math::Vec2F(0.0f, 1.0f) : Math::Vec2F(0.0f, -1.0f);
			}
		}
		break;
		}
	}

	const auto mass1 = CollidingBodies[0].body->Mass, mass2 = CollidingBodies[1].body->Mass;
	const auto rest1 = CollidingBodies[0].collider->Restitution, rest2 = CollidingBodies[1].collider->Restitution;

	Restitution = (mass1 * rest1 + mass2 * rest2) / (mass1 + mass2);

	ResolveVelocityAndInterpenetration();
	ResolveInterpenetration();
}

float Contact::CalculateSeparateVelocity() const noexcept
{
	const auto relativeVelocity = CollidingBodies[0].body->Velocity - CollidingBodies[1].body->Velocity;
	return relativeVelocity.Dot(Normal);
}

void Contact::ResolveVelocityAndInterpenetration() const noexcept
{
	const float separatingVelocity = CalculateSeparateVelocity();

	if (separatingVelocity > 0) {
		return;
	}

	const float newSeparatingVelocity = -separatingVelocity * Restitution;

	const float deltaVelocity = newSeparatingVelocity - separatingVelocity;

	const float inverseMass1 = 1.f / CollidingBodies[0].body->Mass;
	const float inverseMass2 = 1.f / CollidingBodies[1].body->Mass;

	const float totalInverseMass = inverseMass1 + inverseMass2;

	if (totalInverseMass <= 0) {
		return;
	}

	const float impulse = deltaVelocity / totalInverseMass;
	const auto impulsePerIMass = Normal * impulse;

	if (CollidingBodies[0].body->Type == BodyType::DYNAMIC)
	{
		CollidingBodies[0].body->Velocity += impulsePerIMass * inverseMass1;
	}
	if (CollidingBodies[1].body->Type == BodyType::DYNAMIC)
	{
		CollidingBodies[1].body->Velocity -= impulsePerIMass * inverseMass2;
	}

	if (CollidingBodies[0].body->Type == BodyType::STATIC)
	{
		CollidingBodies[1].body->Velocity -= impulsePerIMass * inverseMass1;
	}
	if (CollidingBodies[1].body->Type == BodyType::STATIC)
	{
		CollidingBodies[0].body->Velocity += impulsePerIMass * inverseMass2;
	}
}

void Contact::ResolveInterpenetration() const noexcept
{
	if (Penetration <= 0) return;

	const float inverseMass1 = 1.f / CollidingBodies[0].body->Mass;
	const float inverseMass2 = 1.f / CollidingBodies[1].body->Mass;
	const float totalInverseMass = inverseMass1 + inverseMass2;

	if (totalInverseMass <= 0) {
		return;
	}

	const auto movePerIMass = Normal * (Penetration / totalInverseMass);

	if (CollidingBodies[0].body->Type == BodyType::DYNAMIC)
	{
		CollidingBodies[0].body->Position += movePerIMass * inverseMass1;
	}

	if (CollidingBodies[1].body->Type == BodyType::DYNAMIC)
	{
		CollidingBodies[1].body->Position -= movePerIMass * inverseMass2;
	}
}