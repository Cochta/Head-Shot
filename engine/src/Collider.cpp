#include "Collider.h"

Math::RectangleF Collider::GetBounds() const noexcept
{
	switch (Shape.index())
	{
	case static_cast<int>(Math::ShapeType::Circle):
	{
		auto circle = std::get<Math::CircleF>(Shape);
		return Math::RectangleF::FromCenter(circle.Center(), { circle.Radius(), circle.Radius() }) + BodyPosition;
	}
	case static_cast<int>(Math::ShapeType::Rectangle):
	{
		return std::get<Math::RectangleF>(Shape) + BodyPosition;
	}
	case static_cast<int>(Math::ShapeType::Polygon):
	{
		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::min();
		float maxY = std::numeric_limits<float>::min();
		Math::PolygonF polygon = std::get<Math::PolygonF>(Shape);

		for (auto& vertex : polygon.Vertices())
		{
			minX = std::min(minX, vertex.X);
			minY = std::min(minY, vertex.Y);
			maxX = std::max(maxX, vertex.X);
			maxY = std::max(maxY, vertex.Y);
		}

		return Math::RectangleF{ Math::Vec2F{minX, minY}, Math::Vec2F{maxX, maxY} } + BodyPosition;
	}
	}
	return { Math::Vec2F::Zero(), Math::Vec2F::Zero() };
}

 bool ColliderRefPair::operator==(const ColliderRefPair& other) const
 {
	 return (ColRefA == other.ColRefA && ColRefB == other.ColRefB) ||
		 (ColRefA == other.ColRefB && ColRefB == other.ColRefA);
 }


std::size_t ColliderRefPairHash::operator()(const ColliderRefPair& pair) const
{
	const std::size_t hashA = std::hash<size_t>{}(pair.ColRefA.Index);
	const std::size_t hashB = std::hash<size_t>{}(pair.ColRefB.Index);

	// XOR for the hash
	return hashA ^ hashB;
}
