/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "TextAttributes.h"

#include <react/renderer/attributedstring/conversions.h>
#include <react/renderer/core/conversions.h>
#include <react/renderer/core/graphicsConversions.h>
#include <react/utils/FloatComparison.h>
#include <cmath>

#include <react/renderer/debug/debugStringConvertibleUtils.h>

namespace facebook::react {

#pragma mark - Operators

bool TextAttributes::operator==(const TextAttributes& rhs) const {
  return std::tie(
             foregroundColor,
             backgroundColor,
             fontFamily,
             fontWeight,
             fontStyle,
             fontVariant,
             allowFontScaling,
             dynamicTypeRamp,
             alignment,
             baseWritingDirection,
             lineBreakStrategy,
             textDecorationColor,
             textDecorationLineType,
             textDecorationStyle,
             textShadowOffset,
             textShadowColor,
             isHighlighted,
             isPressable,
             layoutDirection,
             accessibilityRole,
             role,
             textTransform) ==
      std::tie(
             rhs.foregroundColor,
             rhs.backgroundColor,
             rhs.fontFamily,
             rhs.fontWeight,
             rhs.fontStyle,
             rhs.fontVariant,
             rhs.allowFontScaling,
             rhs.dynamicTypeRamp,
             rhs.alignment,
             rhs.baseWritingDirection,
             rhs.lineBreakStrategy,
             rhs.textDecorationColor,
             rhs.textDecorationLineType,
             rhs.textDecorationStyle,
             rhs.textShadowOffset,
             rhs.textShadowColor,
             rhs.isHighlighted,
             rhs.isPressable,
             rhs.layoutDirection,
             rhs.accessibilityRole,
             rhs.role,
             rhs.textTransform) &&
      floatEquality(opacity, rhs.opacity) &&
      floatEquality(fontSize, rhs.fontSize) &&
      floatEquality(fontSizeMultiplier, rhs.fontSizeMultiplier) &&
      floatEquality(letterSpacing, rhs.letterSpacing) &&
      floatEquality(lineHeight, rhs.lineHeight) &&
      floatEquality(textShadowRadius, rhs.textShadowRadius);
}

bool TextAttributes::operator!=(const TextAttributes& rhs) const {
  return !(*this == rhs);
}

#pragma mark - DebugStringConvertible

#if RN_DEBUG_STRING_CONVERTIBLE
SharedDebugStringConvertibleList TextAttributes::getDebugProps() const {
  return {
      // Color
      debugStringConvertibleItem("backgroundColor", backgroundColor),
      debugStringConvertibleItem("foregroundColor", foregroundColor),
      debugStringConvertibleItem("opacity", opacity),

      // Font
      debugStringConvertibleItem("fontFamily", fontFamily),
      debugStringConvertibleItem("fontSize", fontSize),
      debugStringConvertibleItem("fontSizeMultiplier", fontSizeMultiplier),
      debugStringConvertibleItem("fontWeight", fontWeight),
      debugStringConvertibleItem("fontStyle", fontStyle),
      debugStringConvertibleItem("fontVariant", fontVariant),
      debugStringConvertibleItem("allowFontScaling", allowFontScaling),
      debugStringConvertibleItem("dynamicTypeRamp", dynamicTypeRamp),
      debugStringConvertibleItem("letterSpacing", letterSpacing),

      // Paragraph Styles
      debugStringConvertibleItem("lineHeight", lineHeight),
      debugStringConvertibleItem("alignment", alignment),
      debugStringConvertibleItem("baseWritingDirection", baseWritingDirection),
      debugStringConvertibleItem("lineBreakStrategyIOS", lineBreakStrategy),

      // Decoration
      debugStringConvertibleItem("textDecorationColor", textDecorationColor),
      debugStringConvertibleItem(
          "textDecorationLineType", textDecorationLineType),
      debugStringConvertibleItem("textDecorationStyle", textDecorationStyle),

      // Shadow
      debugStringConvertibleItem("textShadowOffset", textShadowOffset),
      debugStringConvertibleItem("textShadowRadius", textShadowRadius),
      debugStringConvertibleItem("textShadowColor", textShadowColor),

      // Special
      debugStringConvertibleItem("isHighlighted", isHighlighted),
      debugStringConvertibleItem("isPressable", isPressable),
      debugStringConvertibleItem("layoutDirection", layoutDirection),
      debugStringConvertibleItem("accessibilityRole", accessibilityRole),
      debugStringConvertibleItem("role", role),
  };
}
#endif

} // namespace facebook::react
