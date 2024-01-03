package com.facebook.react.views.text.attributedstring

import com.facebook.react.common.mapbuffer.MapBuffer
import com.facebook.react.views.text.TextLayoutManagerMapBuffer.AS_KEY_SHARDS

/**
 * [AttributedString] backed by a [MapBuffer]
 */
internal class MapBufferAttributedString(private val attributedString: MapBuffer) : AttributedString {
  override fun getShard(index: Int): AttributedStringShard =
      MapBufferAttributedStringShard(shard = shards.getMapBuffer(index))

  override val shardCount: Int
    get() = shards.count

  private val shards: MapBuffer
    get() = attributedString.getMapBuffer(AS_KEY_SHARDS.toInt())
}
