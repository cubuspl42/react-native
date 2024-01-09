package com.facebook.react.views.text.span

import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.graphics.Typeface
import android.text.Layout
import com.facebook.react.views.text.EffectiveTextAttributeProvider
import com.facebook.react.views.text.FontAttributeProvider
import kotlin.math.max
import kotlin.math.min

internal class ReactShardSpan(
  private val textAttributeProvider: EffectiveTextAttributeProvider,
  private val radius: Float,
) : ReactBackgroundSpan {
  companion object {
    private val backgroundPaint = Paint().apply {
      isAntiAlias = true
      // TODO(cubuspl42): Make this configurable
      color = Color.YELLOW
    }

    private val borderPaint = Paint().apply {
      isAntiAlias = true
      // TODO(cubuspl42): Make this configurable
      color = Color.GRAY
    }
  }

  override val fontAttributeProvider: FontAttributeProvider
    get() = textAttributeProvider

  private val fontSize: Int
    get() = textAttributeProvider.effectiveFontSize

  override fun draw(
    canvas: Canvas,
    layout: Layout,
    start: Int,
    end: Int,
    effectiveTypeface: Typeface,
  ) {
    val startLine: Int = layout.getLineForOffset(start)
    val endLine: Int = layout.getLineForOffset(end)

    assert(endLine >= startLine)

    // Start/end can be on the left or on the right, depending on the language direction
    val startOffset = layout.getPrimaryHorizontal(start)
    val endOffset = layout.getPrimaryHorizontal(end)

    val fontMetrics = Paint().apply {
      typeface = effectiveTypeface
      textSize = fontSize.toFloat()
    }.fontMetrics

    for (line in startLine..endLine) {
      val lineBaseline = layout.getLineBaseline(line)

      val fragmentTop = lineBaseline + fontMetrics.ascent
      val fragmentBottom = lineBaseline + fontMetrics.descent

      val fragmentStart = if (line == startLine) startOffset else layout.getLineLeft(line)
      val fragmentEnd = if (line == endLine) endOffset else layout.getLineRight(line)

      val fragmentLeft = min(fragmentStart, fragmentEnd)
      val fragmentRight = max(fragmentStart, fragmentEnd)

      canvas.drawBorderedRoundRect(
        left = fragmentLeft,
        top = fragmentTop,
        right = fragmentRight,
        bottom = fragmentBottom,
        rx = radius,
        ry = radius,
        backgroundPaint = backgroundPaint,
        borderPaint = borderPaint,
        borderWidth = 2f,
      )
    }
  }
}

private fun Layout.getLineTopWithoutPadding(line: Int): Int {
  var lineTop = getLineTop(line)
  if (line == 0) {
    lineTop -= topPadding
  }
  return lineTop
}

private fun Layout.getLineLeftWithoutPadding(line: Int): Int {
  var lineTop = getLineTop(line)
  if (line == 0) {
    lineTop -= topPadding
  }
  return lineTop
}

private fun Layout.getLineBottomWithoutPadding(line: Int): Int {
  var lineBottom = getLineBottom(line)
  if (line == lineCount - 1) {
    lineBottom -= bottomPadding
  }
  return lineBottom
}

private fun Canvas.drawBorderedRoundRect(
  left: Float,
  top: Float,
  right: Float,
  bottom: Float,
  rx: Float,
  ry: Float,
  backgroundPaint: Paint,
  borderPaint: Paint,
  borderWidth: Float,
) {
  drawRoundRect(
    left,
    top,
    right,
    bottom,
    rx,
    ry,
    borderPaint,
  )

  drawRoundRect(
    left + borderWidth,
    top + borderWidth,
    right - borderWidth,
    bottom - borderWidth,
    rx - borderWidth,
    ry - borderWidth,
    backgroundPaint,
  )
}
