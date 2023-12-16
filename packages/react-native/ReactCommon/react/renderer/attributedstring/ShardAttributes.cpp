/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ShardAttributes.h"

#include <react/renderer/attributedstring/conversions.h>
#include <react/renderer/core/conversions.h>
#include <react/renderer/core/graphicsConversions.h>
#include <react/utils/FloatComparison.h>
#include <cmath>

#include <react/renderer/debug/debugStringConvertibleUtils.h>

namespace facebook::react {

void ShardAttributes::apply(ShardAttributes shardAttributes) {
  backgroundColor = shardAttributes.backgroundColor
      ? shardAttributes.backgroundColor
      : backgroundColor;
}

#pragma mark - Operators

bool ShardAttributes::operator==(const ShardAttributes& rhs) const {
  return std::tie(backgroundColor) ==
      std::tie(rhs.backgroundColor);
}

bool ShardAttributes::operator!=(const ShardAttributes& rhs) const {
  return !(*this == rhs);
}

ShardAttributes ShardAttributes::defaultShardAttributes() {
  static auto shardAttributes = [] {
    auto shardAttributes = ShardAttributes{};
    shardAttributes.backgroundColor = clearColor();
    return shardAttributes;
  }();

  return shardAttributes;
}

#pragma mark - DebugStringConvertible

#if RN_DEBUG_STRING_CONVERTIBLE
SharedDebugStringConvertibleList ShardAttributes::getDebugProps() const {
  return {
      debugStringConvertibleItem("backgroundColor", backgroundColor),
  };
}
#endif

} // namespace facebook::react
