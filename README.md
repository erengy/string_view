# string_view

An incomplete implementation of C++17's `std::string_view`. Not intented for public use, but feel free to take a look.

## String view class

> The class template `basic_string_view` describes an object that can refer to a constant contiguous sequence of char-like objects with the first element of the sequence at position zero.

### Class template

```cpp
template<class charT, class traits = char_traits<charT>>
class basic_string_view;
```

### Typedef names

Alias|Type
-----|----
`string_view`|`basic_string_view<char>`
`u16string_view`|`basic_string_view<char16_t>`
`u32string_view`|`basic_string_view<char32_t>`
`wstring_view`|`basic_string_view<wchar_t>`

## References

- [Working Draft, Standard for Programming Language C++](https://isocpp.org/std/the-standard) *(N4606, 2016-07-12)*
- [std::basic_string_view - cppreference.com](http://en.cppreference.com/w/cpp/string/basic_string_view)
- [libc++/string_view](https://llvm.org/svn/llvm-project/libcxx/trunk/include/string_view)
