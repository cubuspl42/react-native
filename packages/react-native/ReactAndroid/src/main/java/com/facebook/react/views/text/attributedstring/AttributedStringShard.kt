package com.facebook.react.views.text.attributedstring

/**
 * Interface for an attributed string fragment
 */
internal interface AttributedStringShard {
  fun getFragment(index: Int): AttributedStringFragment

  val fragmentCount: Int
}
