/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <folly/Hash.h>
#include <react/renderer/core/LayoutPrimitives.h>
#include <react/renderer/debug/DebugStringConvertible.h>
#include <react/renderer/debug/flags.h>
#include <react/renderer/graphics/Rect.h>
#include <react/renderer/graphics/RectangleEdges.h>

namespace facebook {
namespace react {

/*
 * Describes results of layout process for particular shadow node.
 */
struct LayoutMetrics {
  // Origin: relative to its parent content frame (unless using a method that
  // computes it relative to other parent or the viewport)
  // Size: includes border, padding and content.
  Rect frame;
  // Width of the border + padding in all directions.
  EdgeInsets contentInsets{0};
  // Width of the border in all directions.
  EdgeInsets borderWidth{0};
  DisplayType displayType{DisplayType::Flex};
  LayoutDirection layoutDirection{LayoutDirection::Undefined};
  Float pointScaleFactor{1.0};
  EdgeInsets overflowInset{};

  // Origin: the outer border of the node.
  // Size: includes content only.
  Rect getContentFrame() const {
    return Rect{
        Point{contentInsets.left, contentInsets.top},
        Size{
            frame.size.width - contentInsets.left - contentInsets.right,
            frame.size.height - contentInsets.top - contentInsets.bottom}};
  }

  bool operator==(const LayoutMetrics &rhs) const {
    return std::tie(
               this->frame,
               this->contentInsets,
               this->borderWidth,
               this->displayType,
               this->layoutDirection,
               this->pointScaleFactor,
               this->overflowInset) ==
        std::tie(
               rhs.frame,
               rhs.contentInsets,
               rhs.borderWidth,
               rhs.displayType,
               rhs.layoutDirection,
               rhs.pointScaleFactor,
               rhs.overflowInset);
  }

  bool operator!=(const LayoutMetrics &rhs) const {
    return !(*this == rhs);
  }
};

/*
 * Represents some undefined, not-yet-computed or meaningless value of
 * `LayoutMetrics` type.
 * The value is comparable by equality with any other `LayoutMetrics` value.
 */
static LayoutMetrics const EmptyLayoutMetrics = {
    /* .frame = */ {{0, 0}, {-1.0, -1.0}}};

#ifdef RN_DEBUG_STRING_CONVERTIBLE

std::string getDebugName(LayoutMetrics const &object);
std::vector<DebugStringConvertibleObject> getDebugProps(
    LayoutMetrics const &object,
    DebugStringConvertibleOptions options);

#endif

} // namespace react
} // namespace facebook

namespace std {

template <>
struct hash<facebook::react::LayoutMetrics> {
  size_t operator()(const facebook::react::LayoutMetrics &layoutMetrics) const {
    return folly::hash::hash_combine(
        0,
        layoutMetrics.frame,
        layoutMetrics.contentInsets,
        layoutMetrics.borderWidth,
        layoutMetrics.displayType,
        layoutMetrics.layoutDirection,
        layoutMetrics.pointScaleFactor,
        layoutMetrics.overflowInset);
  }
};

} // namespace std
