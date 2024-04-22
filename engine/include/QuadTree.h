#pragma once

#include <memory>

#include "Allocators.h"
#include "Collider.h"

/**
 * @brief Structure representing a bounding box (AABB) associated with a
 * collider reference.
 */
struct ColliderRefAabb {
  Math::RectangleF Aabb; /**< The bounding box (AABB). */
  ColliderRef ColRef;    /**< The reference to a collider. */
};

/**
 * @brief Class representing a node in a quadtree data structure for collision
 * detection.
 */
class QuadNode {
 public:
  CustomlyAllocatedVector<ColliderRefAabb>
      ColliderRefAabbs; /**< Vector of collider references with AABBs. */
  Math::RectangleF Bounds{
      Math::Vec2F::Zero(),
      Math::Vec2F::Zero()}; /**< The bounds of the quadtree node. */
  std::array<QuadNode*, 4> Children{nullptr, nullptr, nullptr,
                                    nullptr}; /**< Array of child nodes. */
  int Depth = 0; /**< The depth of the node in the quadtree.*/

  /**
   * @brief Constructor for QuadNode, allocating memory using a specified
   * allocator.
   * @param alloc The allocator for memory allocation.
   */
  QuadNode(Allocator& alloc) noexcept;

  /**
   * @brief Constructor for QuadNode with a specified bounds, allocating memory
   * using a specified allocator.
   * @param bounds The bounds of the quadtree node.
   * @param alloc The allocator for memory allocation.
   */
  explicit QuadNode(const Math::RectangleF& bounds, Allocator& alloc) noexcept;
};

/**
 * @brief Class representing a quadtree data structure for collision detection.
 */
class QuadTree {
 public:
 private:
  static constexpr int MAX_COL_NBR =
      16; /**< Maximum number of colliders in a quadtree node. */
  static constexpr int MAX_DEPTH = 5; /**< Maximum depth of the quadtree. */
  int _nodeIndex = 1;   /**< The index of the current node in the quadtree. */
  HeapAllocator _alloc; /**< The allocator for memory allocation.*/
 public:
  CustomlyAllocatedVector<QuadNode> Nodes{
      StandardAllocator<QuadNode>{_alloc}}; /**< Vector of quadtree nodes. */
  /**
   * @brief Constructor for QuadTree, allocating memory using a specified
   * allocator.
   * @param alloc The allocator for memory allocation.
   */
  QuadTree() noexcept;

  /**
   * @brief Set up the root of the quadtree with the specified bounds.
   * @param bounds The bounds of the root node.
   */
  void SetUpRoot(const Math::RectangleF& bounds) noexcept;

  /**
   * @brief Insert a collider reference with an associated AABB into the
   * quadtree.
   * @param node The node to insert into.
   * @param colliderRefAabb The collider reference with an AABB to insert.
   */
  void Insert(QuadNode& node, const ColliderRefAabb& colliderRefAabb) noexcept;

 private:
  /**
   * @brief Subdivide a quadtree node into smaller child nodes.
   * @param node The node to subdivide.
   */
  void SubdivideNode(QuadNode& node) noexcept;
};