package com.facebook.react.views.text.attributedstring

import com.facebook.react.views.text.TextAttributeProps

/**
 * Interface for an attributed string fragment
 */
internal interface AttributedStringFragment {
  val textAttributeProps: TextAttributeProps

  val string: String?

  fun hasReactTag(): Boolean

  val reactTag: Int

  fun hasIsAttachment(): Boolean

  val isAttachment: Boolean

  val width: Double

  val height: Double
}
