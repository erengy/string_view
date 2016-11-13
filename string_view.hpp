#pragma once

#include <algorithm>
#include <iterator>
#include <limits>
#include <string>
#include <ostream>
#include <stdexcept>
#include <utility>

// TODO: Check relaxed constexpr restrictions
#define constexpr14

namespace erengy {

template<class charT, class traits = std::char_traits<charT>>
class basic_string_view {
public:
  //////////////////////////////////////////////////////////////////////////////
  // Types

  using traits_type = traits;
  using value_type = charT;
  using pointer = charT*;
  using const_pointer = const charT*;
  using reference = charT&;
  using const_reference = const charT&;
  using const_iterator = const_pointer;  // implementation-defined
  using iterator = const_iterator;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reverse_iterator = const_reverse_iterator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  //////////////////////////////////////////////////////////////////////////////
  // Constants

  static constexpr size_type npos = size_type(-1);

  //////////////////////////////////////////////////////////////////////////////
  // Construction and assignment

  constexpr basic_string_view() noexcept
      : data_(nullptr), size_(0) {
  }

  constexpr basic_string_view(const basic_string_view&) noexcept = default;

  basic_string_view& operator=(const basic_string_view&) noexcept = default;

  constexpr basic_string_view(const_pointer str)
      : data_(str), size_(traits::length(str)) {
    // Requires: [str, str + traits::length(str)) is a valid range.
  }

  constexpr basic_string_view(const_pointer str, size_type len)
      : data_(str), size_(len) {
    // Requires: [str, str + len) is a valid range.
  }

  template<class allocator>
  constexpr basic_string_view(const std::basic_string<charT, traits, allocator>& str) noexcept
      : data_(str.data()), size_(str.size()) {
  }

  //////////////////////////////////////////////////////////////////////////////
  // Iterator support

  constexpr const_iterator begin() const noexcept {
    return cbegin();
  }

  constexpr const_iterator end() const noexcept {
    return cend();
  }

  constexpr const_iterator cbegin() const noexcept {
    return data_;
  }

  constexpr const_iterator cend() const noexcept {
    return cbegin() + size();
  }

  const_reverse_iterator rbegin() const noexcept {
    return crbegin();
  }

  const_reverse_iterator rend() const noexcept {
    return crend();
  }

  const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(cend());
  }

  const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(cbegin());
  }

  //////////////////////////////////////////////////////////////////////////////
  // Capacity

  constexpr size_type size() const noexcept {
    return size_;
  }

  constexpr size_type length() const noexcept {
    return size_;
  }

  constexpr size_type max_size() const noexcept {
    return std::numeric_limits<size_type>::max();
  }

  constexpr bool empty() const noexcept {
    return size_ == 0;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Element access

  constexpr const_reference operator[](size_type pos) const {
    // Requires: pos < size().
    return data_[pos];
  }

  constexpr const_reference at(size_type pos) const {
    if (pos >= size()) {
      throw std::out_of_range("basic_string_view::at");
    }
    return data_[pos];
  }

  constexpr const_reference front() const {
    // Requires: !empty().
    return data_[0];
  }

  constexpr const_reference back() const {
    // Requires: !empty().
    return data_[size() - 1];
  }

  constexpr const_pointer data() const noexcept {
    return data_;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Modifiers

  constexpr14 void remove_prefix(size_type n) {
    // Requires: n <= size().
    data_ += n;
    size_ -= n;
  }

  constexpr14 void remove_suffix(size_type n) {
    // Requires: n <= size().
    size_ -= n;
  }

  constexpr14 void swap(basic_string_view& s) noexcept {
    std::swap(*this, s);
  }

  //////////////////////////////////////////////////////////////////////////////
  // String operations

  size_type copy(pointer s, size_type n, size_type pos = 0) const {
    if (pos > size()) {
      throw std::out_of_range("basic_string_view::copy");
    }
    // Requires: [s, s + rlen) is a valid range.
    const auto rlen = std::min(n, size() - pos);
    std::copy_n(begin() + pos, rlen, s);
    return rlen;
  }

  constexpr basic_string_view substr(size_type pos = 0, size_type n = npos) const {
    if (pos > size()) {
      throw std::out_of_range("basic_string_view::substr");
    }
    return basic_string_view(data() + pos,
                             std::min(n, size() - pos));  // rlen
  }

  constexpr14 int compare(basic_string_view str) const noexcept {
    const auto rlen = std::min(size(), str.size());
    auto result = traits::compare(data(), str.data(), rlen);
    if (result == 0)
      result = size() == str.size() ? 0 : (size() < str.size() ? -1 : 1);
    return result;
  }
  constexpr14 int compare(size_type pos1, size_type n1, basic_string_view s) const {
    return substr(pos1, n1).compare(str);
  }
  constexpr14 int compare(size_type pos1, size_type n1, basic_string_view s,
                          size_type pos2, size_type n2) const {
    return substr(pos1, n1).compare(str.substr(pos2, n2));
  }
  constexpr14 int compare(const_pointer s) const {
    return compare(basic_string_view(s));
  }
  constexpr14 int compare(size_type pos1, size_type n1, const_pointer s) const {
    return substr(pos1, n1).compare(basic_string_view(s));
  }
  constexpr14 int compare(size_type pos1, size_type n1, const_pointer s, size_type n2) const {
    return substr(pos1, n1).compare(basic_string_view(s, n2));
  }

  constexpr14 size_type find(basic_string_view s, size_type pos = 0) const noexcept {
    auto it = std::search(begin() + pos, end(), s.begin(), s.end());
    return it != end() ? std::distance(begin(), it) : npos;
  }
  constexpr14 size_type find(value_type c, size_type pos = 0) const noexcept {
    return find(basic_string_view(&c, 1), pos);
  }
  constexpr14 size_type find(const_pointer s, size_type pos, size_type n) const {
    return find(basic_string_view(s, n), pos);
  }
  constexpr14 size_type find(const_pointer s, size_type pos = 0) const {
    return find(basic_string_view(s), pos);
  }

  constexpr14 size_type rfind(basic_string_view s, size_type pos = npos) const noexcept {
    auto last = (pos == npos) ? end() : begin() + pos;
    auto it = std::find_end(begin(), last, s.begin(), s.end());
    return it != last ? std::distance(begin(), it) : npos;
  }
  constexpr14 size_type rfind(value_type c, size_type pos = npos) const noexcept {
    return rfind(basic_string_view(&c, 1), pos);
  }
  constexpr14 size_type rfind(const_pointer s, size_type pos, size_type n) const {
    return rfind(basic_string_view(s, n), pos);
  }
  constexpr14 size_type rfind(const_pointer s, size_type pos = npos) const {
    return rfind(basic_string_view(s), pos);
  }

  constexpr14 size_type find_first_of(basic_string_view s, size_type pos = 0) const noexcept {
    auto it = std::find_first_of(begin() + pos, end(), s.begin(), s.end());
    return it != end() ? std::distance(begin(), it) : npos;
  }
  constexpr14 size_type find_first_of(value_type c, size_type pos = 0) const noexcept {
    return find_first_of(basic_string_view(&c, 1), pos);
  }
  constexpr14 size_type find_first_of(const_pointer s, size_type pos, size_type n) const {
    return find_first_of(basic_string_view(s, n), pos);
  }
  constexpr14 size_type find_first_of(const_pointer s, size_type pos = 0) const {
    return find_first_of(basic_string_view(s), pos);
  }

  constexpr14 size_type find_last_of(basic_string_view s, size_type pos = npos) const noexcept {
    auto it = std::find_first_of(rbegin(), rend() - pos, s.begin(), s.end());
    return it != rend() ? std::distance(begin(), it.base()) - 1 : npos;
  }
  constexpr14 size_type find_last_of(value_type c, size_type pos = npos) const noexcept {
    return find_last_of(basic_string_view(&c, 1), pos);
  }
  constexpr14 size_type find_last_of(const_pointer s, size_type pos, size_type n) const {
    return find_last_of(basic_string_view(s, n), pos);
  }
  constexpr14 size_type find_last_of(const_pointer s, size_type pos = npos) const {
    return find_last_of(basic_string_view(s), pos);
  }

  constexpr14 size_type find_first_not_of(basic_string_view s, size_type pos = 0) const noexcept {
    auto it = std::find_if(begin() + pos, end(),
        [&s](const value_type& c) {
          return std::find(s.begin(), s.end(), c) == s.end();
        });
    return it != end() ? std::distance(begin(), it) : npos;
  }
  constexpr14 size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept {
    return find_first_not_of(basic_string_view(&c, 1), pos);
  }
  constexpr14 size_type find_first_not_of(const_pointer s, size_type pos, size_type n) const {
    return find_first_not_of(basic_string_view(s, n), pos);
  }
  constexpr14 size_type find_first_not_of(const_pointer s, size_type pos = 0) const {
    return find_first_not_of(basic_string_view(s), pos);
  }

  constexpr14 size_type find_last_not_of(basic_string_view s, size_type pos = npos) const noexcept {
    auto it = std::find_if(rbegin(), rend() - pos,
        [&s](const value_type& c) {
          return std::find(s.begin(), s.end(), c) == s.end();
        });
    return it != rend() ? std::distance(begin(), it.base()) - 1 : npos;
  }
  constexpr14 size_type find_last_not_of(value_type c, size_type pos = npos) const noexcept {
    return find_last_not_of(basic_string_view(&c, 1), pos);
  }
  constexpr14 size_type find_last_not_of(const_pointer s, size_type pos, size_type n) const {
    return find_last_not_of(basic_string_view(s, n), pos);
  }
  constexpr14 size_type find_last_not_of(const_pointer s, size_type pos = npos) const {
    return find_last_not_of(basic_string_view(s), pos);
  }

private:
  const_pointer data_;
  size_type size_;
};

////////////////////////////////////////////////////////////////////////////////
// Non-member comparison functions
// TODO: Provide sufficient additional overloads

template<class charT, class traits>
constexpr14 bool operator==(basic_string_view<charT, traits> lhs,
                            basic_string_view<charT, traits> rhs) noexcept {
  return lhs.compare(rhs) == 0;
}

template<class charT, class traits>
constexpr14 bool operator!=(basic_string_view<charT, traits> lhs,
                            basic_string_view<charT, traits> rhs) noexcept {
  return lhs.compare(rhs) != 0;
}

template<class charT, class traits>
constexpr14 bool operator< (basic_string_view<charT, traits> lhs,
                            basic_string_view<charT, traits> rhs) noexcept {
  return lhs.compare(rhs) < 0;
}

template<class charT, class traits>
constexpr14 bool operator> (basic_string_view<charT, traits> lhs,
                            basic_string_view<charT, traits> rhs) noexcept {
  return lhs.compare(rhs) > 0;
}

template<class charT, class traits>
constexpr14 bool operator<=(basic_string_view<charT, traits> lhs,
                            basic_string_view<charT, traits> rhs) noexcept {
  return lhs.compare(rhs) <= 0;
}

template<class charT, class traits>
constexpr14 bool operator>=(basic_string_view<charT, traits> lhs,
                            basic_string_view<charT, traits> rhs) noexcept {
  return lhs.compare(rhs) >= 0;
}

////////////////////////////////////////////////////////////////////////////////
// Inserters and extractors

template<class charT, class traits>
std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os,
                                              basic_string_view<charT, traits> str) {
  return os << str.to_string();
}

////////////////////////////////////////////////////////////////////////////////
// Typedef names

using string_view = basic_string_view<char>;
using u16string_view = basic_string_view<char16_t>;
using u32string_view = basic_string_view<char32_t>;
using wstring_view = basic_string_view<wchar_t>;

////////////////////////////////////////////////////////////////////////////////
// TODO: Hash support

template<class T> struct hash;
template<> struct hash<string_view>;
template<> struct hash<u16string_view>;
template<> struct hash<u32string_view>;
template<> struct hash<wstring_view>;

}  // namespace erengy
