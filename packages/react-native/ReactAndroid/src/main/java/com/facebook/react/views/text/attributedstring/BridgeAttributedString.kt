package com.facebook.react.views.text.attributedstring

import com.facebook.react.bridge.ReadableArray
import com.facebook.react.bridge.ReadableMap

/**
 * [AttributedString] backed by a [ReadableMap]
 */
internal class BridgeAttributedString(private val attributedString: ReadableMap) : AttributedString {
  override fun getFragment(index: Int): AttributedStringFragment = BridgeAttributedStringFragment(fragments.getMap(index))

  override val fragmentCount: Int
    get() = fragments.size()

  private val fragments: ReadableArray
    get() = attributedString.getArray("fragments")!!
}
