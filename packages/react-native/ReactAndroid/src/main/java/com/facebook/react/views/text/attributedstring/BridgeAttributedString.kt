package com.facebook.react.views.text.attributedstring

import com.facebook.react.bridge.ReadableArray
import com.facebook.react.bridge.ReadableMap

/**
 * [AttributedString] backed by a [ReadableMap]
 */
internal class BridgeAttributedString(private val attributedString: ReadableMap) : AttributedString {
  override fun getShard(index: Int): AttributedStringShard = BridgeAttributedStringShard(shard = shards.getMap(index))

  override val shardCount: Int
    get() = shards.size()

  private val shards: ReadableArray
    get() = attributedString.getArray("shards")!!
}
