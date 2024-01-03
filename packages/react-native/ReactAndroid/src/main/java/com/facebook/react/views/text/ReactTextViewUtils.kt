package com.facebook.react.views.text

import android.graphics.Typeface
import android.widget.TextView

fun TextView.findEffectiveTypeface(fontAttributeProvider: FontAttributeProvider): Typeface =
  fontAttributeProvider.findEffectiveTypeface(
    baseTypeface = typeface,
    assetManager = context.assets,
  )
