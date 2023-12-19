package com.facebook.react.views.text.attributedstring

/**
 * Interface for an attributed string
 */
internal interface AttributedString {
  fun getFragment(index: Int): AttributedStringFragment

  val fragmentCount: Int
}
