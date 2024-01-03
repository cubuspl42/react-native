package com.facebook.react.views.text.attributedstring

/**
 * Interface for an attributed string
 */
internal interface AttributedString {
  fun getShard(index: Int): AttributedStringShard

  val shardCount: Int
}
