package com.facebook.react.views.text

import android.content.res.AssetManager
import android.graphics.Typeface

interface FontAttributeProvider {
  val fontFamily: String?

  val fontStyle: Int

  val fontWeight: Int
}

fun FontAttributeProvider.findEffectiveTypeface(
  baseTypeface: Typeface?,
  assetManager: AssetManager,
): Typeface = ReactTypefaceUtils.applyStyles(
  baseTypeface,
  fontStyle,
  fontWeight,
  fontFamily,
  assetManager,
)
