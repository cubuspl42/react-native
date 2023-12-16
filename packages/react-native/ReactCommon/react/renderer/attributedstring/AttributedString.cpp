/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AttributedString.h"

#include <react/renderer/debug/DebugStringConvertibleItem.h>

namespace facebook::react {

using Fragment = AttributedString::Fragment;
using Fragments = AttributedString::Fragments;
using FragmentHandle = AttributedString::FragmentHandle;

using Shard = AttributedString::Shard;
using Shards = AttributedString::Shards;

#pragma mark - Fragment

std::string Fragment::AttachmentCharacter() {
  // C++20 makes char8_t a distinct type from char, and u8 string literals
  // consist of char8_t instead of char, which in turn requires std::u8string,
  // etc. Here we were assuming char was UTF-8 anyway, so just cast to that
  // (which is valid because char* is allowed to alias anything).
  return reinterpret_cast<const char*>(
      u8"\uFFFC"); // Unicode `OBJECT REPLACEMENT CHARACTER`
}

bool Fragment::isAttachment() const {
  return string == AttachmentCharacter();
}

bool Fragment::operator==(const Fragment& rhs) const {
  return std::tie(
             string,
             textAttributes,
             parentShadowView.tag,
             parentShadowView.layoutMetrics) ==
      std::tie(
             rhs.string,
             rhs.textAttributes,
             rhs.parentShadowView.tag,
             rhs.parentShadowView.layoutMetrics);
}

bool Fragment::isContentEqual(const Fragment& rhs) const {
  return std::tie(string, textAttributes) ==
      std::tie(rhs.string, rhs.textAttributes);
}

bool Fragment::operator!=(const Fragment& rhs) const {
  return !(*this == rhs);
}

#pragma mark - Shard

const Fragments& Shard::getFragments() const {
  return fragments_;
}

Fragments& Shard::getFragments() {
  return fragments_;
}

std::string Shard::getString() const {
  auto string = std::string{};
  for (const auto& fragment : fragments_) {
    string += fragment.string;
  }
  return string;
}

void Shard::appendFragment(const Fragment& fragment) {
  ensureUnsealed();

  if (fragment.string.empty()) {
    return;
  }

  fragments_.push_back(fragment);
}

void Shard::prependFragment(const Fragment& fragment) {
  ensureUnsealed();

  if (fragment.string.empty()) {
    return;
  }

  fragments_.insert(fragments_.begin(), fragment);
}

bool Shard::isEmpty() const {
  return fragments_.empty();
}

bool Shard::compareTextAttributesWithoutFrame(const Shard& rhs) const {
  if (fragments_.size() != rhs.fragments_.size()) {
    return false;
  }

  for (size_t i = 0; i < fragments_.size(); i++) {
    if (fragments_[i].textAttributes != rhs.fragments_[i].textAttributes ||
        fragments_[i].string != rhs.fragments_[i].string) {
      return false;
    }
  }

  return true;
}

bool Shard::isContentEqual(const Shard& rhs) const {
  if (fragments_.size() != rhs.fragments_.size()) {
    return false;
  }

  for (size_t i = 0; i < fragments_.size(); i++) {
    if (!fragments_[i].isContentEqual(rhs.fragments_[i])) {
      return false;
    }
  }

  return true;
}

int Shard::countAttachments() const {
  int count = 0;

  for (const auto& fragment : fragments_) {
    if (fragment.isAttachment()) {
      count++;
    }
  }

  return count;
}

bool Shard::operator==(const Shard& rhs) const {
  return fragments_ == rhs.fragments_;
}

bool Shard::operator!=(const Shard& rhs) const {
  return !(*this == rhs);
}

#pragma mark - AttributedString

void AttributedString::appendShard(const Shard& shard) {
  ensureUnsealed();

  if (shard.isEmpty()) {
    return;
  }

  shards_.push_back(shard);
}

void AttributedString::prependShard(const Shard& shard) {
  ensureUnsealed();

  if (shard.isEmpty()) {
    return;
  }

  shards_.insert(shards_.begin(), shard);
}

FragmentHandle AttributedString::appendFragment(const Fragment &fragment) {
  ensureUnsealed();

  // TODO(cubuspl42): Make this work for multiple shards
  Shard &root = shards_.front();
  root.appendFragment(fragment);

  auto fragmentIndex = root.getFragments().size() - 1;

  return FragmentHandle{0, fragmentIndex};
}

FragmentHandle AttributedString::prependFragment(const Fragment &fragment) {
  ensureUnsealed();

  // TODO(cubuspl42): Make this work for multiple shards
  shards_.front().prependFragment(fragment);

  return FragmentHandle{0, 0};
}

void AttributedString::appendAttributedString(
    const AttributedString& attributedString) {
  ensureUnsealed();
  shards_.insert(
      shards_.end(),
      attributedString.shards_.begin(),
      attributedString.shards_.end());
}

void AttributedString::prependAttributedString(
    const AttributedString& attributedString) {
  ensureUnsealed();
  shards_.insert(
      shards_.begin(),
      attributedString.shards_.begin(),
      attributedString.shards_.end());
}

int AttributedString::countAllAttachments() const {
  int count = 0;

  for (const auto& shard : shards_) {
    count += shard.countAttachments();
  }

  return count;
}

const Shards& AttributedString::getShards() const {
  return shards_;
}

Shards& AttributedString::getShards() {
  return shards_;
}

const Fragments AttributedString::getAllFragments() const {
  auto fragments = Fragments{};

  for (const auto& shard : shards_) {
    for (const auto& fragment : shard.getFragments()) {
      fragments.push_back(fragment);
    }
  }

  return fragments;
}

Fragment& AttributedString::getFragment(FragmentHandle handle) {
  return getShards()[handle.shardIndex].getFragments()[handle.fragmentIndex];
}

const Fragment& AttributedString::getFragment(FragmentHandle handle) const {
  return getShards()[handle.shardIndex].getFragments()[handle.fragmentIndex];
}

std::string AttributedString::getJoinedString() const {
  auto string = std::string{};
  for (const auto& shard : shards_) {
    string += shard.getString();
  }
  return string;
}

bool AttributedString::isEmpty() const {
  return shards_.empty();
}

bool AttributedString::compareTextAttributesWithoutFrame(
    const AttributedString& rhs) const {
  if (shards_.size() != rhs.shards_.size()) {
    return false;
  }

  for (size_t i = 0; i < shards_.size(); i++) {
    if (!shards_[i].compareTextAttributesWithoutFrame(rhs.shards_[i])) {
      return false;
    }
  }

  return true;
}

bool AttributedString::isContentEqual(const AttributedString& rhs) const {
  if (shards_.size() != rhs.shards_.size()) {
    return false;
  }

  for (size_t i = 0; i < shards_.size(); i++) {
    if (!shards_[i].isContentEqual(rhs.shards_[i])) {
      return false;
    }
  }

  return true;
}

void AttributedString::sealChildren() const {
  for (const Shard& shard : shards_) {
    shard.seal();
  }
}

bool AttributedString::operator==(const AttributedString& rhs) const {
  return shards_ == rhs.shards_;
}

bool AttributedString::operator!=(const AttributedString& rhs) const {
  return !(*this == rhs);
}

#pragma mark - DebugStringConvertible

#if RN_DEBUG_STRING_CONVERTIBLE
SharedDebugStringConvertibleList Shard::getDebugChildren() const {
  auto list = SharedDebugStringConvertibleList{};

  for (auto&& fragment : fragments_) {
    auto propsList =
        fragment.textAttributes.DebugStringConvertible::getDebugProps();

    list.push_back(std::make_shared<DebugStringConvertibleItem>(
        "Fragment",
        fragment.string,
        SharedDebugStringConvertibleList(),
        propsList));
  }

  return list;
}

SharedDebugStringConvertibleList AttributedString::getDebugChildren() const {
  auto list = SharedDebugStringConvertibleList{};

  for (auto&& shard : shards_) {
    list.push_back(
        std::make_shared<DebugStringConvertibleItem>("Shard", shard.getString()));
  }

  return list;
}

#endif

} // namespace facebook::react
