package com.facebook.react.views.text.attributedstring

import com.facebook.react.bridge.ReadableArray
import com.facebook.react.bridge.ReadableMap

/**
 * [AttributedStringShard] backed by a [ReadableMap]
 */
internal class BridgeAttributedStringShard(private val shard: ReadableMap) : AttributedStringShard {
  override fun getFragment(index: Int): BridgeAttributedStringFragment = BridgeAttributedStringFragment(fragment = fragments.getMap(index))

  override val fragmentCount: Int
    get() = fragments.size()

  private val fragments: ReadableArray
    get() = shard.getArray("fragments")!!
}
