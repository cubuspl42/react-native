/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <memory>

#include <react/renderer/attributedstring/TextAttributes.h>
#include <react/renderer/core/Sealable.h>
#include <react/renderer/core/ShadowNode.h>
#include <react/renderer/debug/DebugStringConvertible.h>
#include <react/renderer/mounting/ShadowView.h>
#include <react/utils/hash_combine.h>

namespace facebook::react {

class AttributedString;

using SharedAttributedString = std::shared_ptr<const AttributedString>;

/*
 * A simple, cross-platform, React-specific implementation of an attributed
 * string (also known as spanned string).
 */
class AttributedString : public Sealable, public DebugStringConvertible {
 public:
  /*
   * A Fragment represents a part of the AttributedString with its own set of
   * attributes.
   */
  class Fragment {
   public:
    static std::string AttachmentCharacter();

    /*
     * The encapsulated text fragment content.
     */
    std::string string;

    /*
     * Defines the attributes (like font, color, size etc.) of the text
     * fragment.
     */
    TextAttributes textAttributes;

    /*
     * The `ShadowView` that is associated with the encapsulated text fragment.
     */
    ShadowView parentShadowView;

    /*
     * Returns true is the Fragment represents an attachment.
     * Equivalent to `string == AttachmentCharacter()`.
     */
    bool isAttachment() const;

    /*
     * Returns whether the underlying text and attributes are equal,
     * disregarding layout or other information.
     */
    bool isContentEqual(const Fragment& rhs) const;

    bool operator==(const Fragment& rhs) const;
    bool operator!=(const Fragment& rhs) const;
  };

  /*
   * A Shard represents a sequence of Fragments in the AttributedString.
   * It is responsible for carrying styles that apply to multiple fragments
   * (e.g. borders).
   */
  class Shard : public Sealable, public DebugStringConvertible {
   public:
    using Fragments = std::vector<Fragment>;

    /*
     * Returns a read-only reference to a list of fragments.
     */
    const Fragments& getFragments() const;

    /*
     * Returns a reference to a list of fragments.
     */
    Fragments& getFragments();

    /*
     * Returns a string constructed from the fragments.
     */
    std::string getString() const;

    /*
     * Returns `true` if the shard is empty (has no fragments).
     */
    bool isEmpty() const;

    /*
     * Compares equality of TextAttributes of all Fragments on both sides.
     */
    bool compareTextAttributesWithoutFrame(const Shard& rhs) const;

    bool isContentEqual(const Shard& rhs) const;

    /*
     * Returns the number of attachments in the shard.
     */
    int countAttachments() const;

    /*
     * Appends and prepends a `fragment` to the string.
     */
    void appendFragment(const Fragment& fragment);
    void prependFragment(const Fragment& fragment);


    bool operator==(const Shard& rhs) const;
    bool operator!=(const Shard& rhs) const;

#pragma mark - DebugStringConvertible

#if RN_DEBUG_STRING_CONVERTIBLE
    SharedDebugStringConvertibleList getDebugChildren() const override;
#endif

    virtual ~Shard() = default;

   private:
    Fragments fragments_;
  };

  /*
   * A helper class that holds indices to locate a specific fragment within an
   * attributed string.
   */
  class FragmentHandle final {
   public:
    const size_t shardIndex;
    const size_t fragmentIndex;

    FragmentHandle(size_t shardIndex, size_t fragmentIndex)
        : shardIndex{shardIndex}, fragmentIndex{fragmentIndex} {}
  };

  class Range {
   public:
    int location{0};
    int length{0};
  };

  using Fragments = std::vector<Fragment>;
  using Shards = std::vector<Shard>;

  /*
   * Appends and prepends a shard to the attributed string.
   */
  void appendShard(const Shard& shard);
  void prependShard(const Shard& shard);

  /*
   * Appends a `fragment` to the last shard.
   */
  FragmentHandle appendFragment(const Fragment& fragment);

  /*
   * Prepends a `fragment` to the first shard.
   */
  FragmentHandle prependFragment(const Fragment& fragment);

  /*
   * Appends and prepends an `attributedString` (all its shards) to
   * the string.
   */
  void appendAttributedString(const AttributedString& attributedString);
  void prependAttributedString(const AttributedString& attributedString);

  /*
   * Returns the number of attachments in the attributed string.
   */
  int countAllAttachments() const;

  /*
   * Returns a read-only reference to a list of shards.
   */
  const Shards& getShards() const;

  /*
   * Returns a reference to a list of shards.
   */
  Shards& getShards();

  /*
   * Returns a list of all fragments in the attributed string.
   */
  const Fragments getAllFragments() const;

  Fragment& getFragment(FragmentHandle handle);

  const Fragment& getFragment(FragmentHandle handle) const;

  /*
   * Returns a concatanated string constructed from all shards.
   */
  std::string getJoinedString() const;

  /*
   * Returns `true` if the attributed string is empty (has no shards).
   */
  bool isEmpty() const;

  /*
   * Compares equality of TextAttributes of all Fragments on both sides.
   */
  bool compareTextAttributesWithoutFrame(const AttributedString& rhs) const;

  bool isContentEqual(const AttributedString& rhs) const;

 protected:
  virtual void sealChildren() const override;

 public:
  bool operator==(const AttributedString& rhs) const;
  bool operator!=(const AttributedString& rhs) const;

#pragma mark - DebugStringConvertible

#if RN_DEBUG_STRING_CONVERTIBLE
  SharedDebugStringConvertibleList getDebugChildren() const override;
#endif

 private:
  Shards shards_{AttributedString::Shard{}};
};

} // namespace facebook::react

namespace std {
template <>
struct hash<facebook::react::AttributedString::Fragment> {
  size_t operator()(
      const facebook::react::AttributedString::Fragment& fragment) const {
    return facebook::react::hash_combine(
        fragment.string,
        fragment.textAttributes,
        fragment.parentShadowView,
        fragment.parentShadowView.layoutMetrics);
  }
};

template <>
struct hash<facebook::react::AttributedString::Shard> {
  size_t operator()(const facebook::react::AttributedString::Shard& shard) const {
    auto seed = size_t{0};

    for (const auto& fragment : shard.getFragments()) {
      facebook::react::hash_combine(seed, fragment);
    }

    return seed;
  }
};

template <>
struct hash<facebook::react::AttributedString> {
  size_t operator()(
      const facebook::react::AttributedString& attributedString) const {
    auto seed = size_t{0};

    for (const auto& shard : attributedString.getShards()) {
      facebook::react::hash_combine(seed, shard);
    }

    return seed;
  }
};
} // namespace std
