/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <functional>
#include <limits>
#include <optional>

#include <react/renderer/attributedstring/primitives.h>
#include <react/renderer/components/view/AccessibilityPrimitives.h>
#include <react/renderer/core/LayoutPrimitives.h>
#include <react/renderer/core/ReactPrimitives.h>
#include <react/renderer/debug/DebugStringConvertible.h>
#include <react/renderer/graphics/Color.h>
#include <react/renderer/graphics/Float.h>
#include <react/renderer/graphics/Size.h>
#include <react/utils/hash_combine.h>

namespace facebook::react {

class ShardAttributes;

using SharedShardAttributes = std::shared_ptr<const ShardAttributes>;

class ShardAttributes : public DebugStringConvertible {
 public:
  /*
   * Returns ShardAttributes object which has actual default attribute values,
   * in oppose to ShardAttributes's default constructor which creates an object
   * with non-set attributes.
   */
  static ShardAttributes defaultShardAttributes();

#pragma mark - Fields

  SharedColor backgroundColor{};

#pragma mark - Operations

  void apply(ShardAttributes ShardAttributes);

#pragma mark - Operators

  bool operator==(const ShardAttributes& rhs) const;
  bool operator!=(const ShardAttributes& rhs) const;

#pragma mark - DebugStringConvertible

#if RN_DEBUG_STRING_CONVERTIBLE
  SharedDebugStringConvertibleList getDebugProps() const override;
#endif
};

} // namespace facebook::react

namespace std {

template <>
struct hash<facebook::react::ShardAttributes> {
  size_t operator()(
      const facebook::react::ShardAttributes& ShardAttributes) const {
    return facebook::react::hash_combine(ShardAttributes.backgroundColor);
  }
};

} // namespace std
