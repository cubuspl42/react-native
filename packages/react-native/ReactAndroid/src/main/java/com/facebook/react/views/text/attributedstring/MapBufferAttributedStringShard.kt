package com.facebook.react.views.text.attributedstring

import com.facebook.react.common.mapbuffer.MapBuffer
import com.facebook.react.views.text.TextLayoutManagerMapBuffer.SH_KEY_FRAGMENTS

/**
 * [AttributedStringShard] backed by a [MapBuffer]
 */
internal class MapBufferAttributedStringShard(private val shard: MapBuffer) : AttributedStringShard {
  // TODO(cubuspl42): Expose shard attributes

  override fun getFragment(index: Int): AttributedStringFragment =
      MapBufferAttributedStringFragment(fragment = fragments.getMapBuffer(index))

  override val fragmentCount: Int
    get() = fragments.count

  private val fragments: MapBuffer
    get() = shard.getMapBuffer(SH_KEY_FRAGMENTS.toInt())
}
