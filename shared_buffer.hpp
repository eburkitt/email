//
// Created by evanb on 12/3/17.
//

#ifndef EMAIL_SHARED_BUFFER_HPP
#define EMAIL_SHARED_BUFFER_HPP

#include <vector>
#include <memory>
#include <stdexcept>

namespace els {
namespace util {

template <typename CharT, typename Traits = ::std::char_traits<CharT>>
class basic_shared_buffer
{
public:
    basic_shared_buffer() {}
    explicit basic_shared_buffer(::std::string const &s)
            : buffer_(::std::make_shared<buffer_t>(s.cbegin(), s.cend())) {}
    //TODO ctor taking pointer should check it for null
    basic_shared_buffer(CharT const *c, ::std::size_t len)
            : buffer_(::std::make_shared(c, c + len)) {}

    //TODO should default-constructed instance throw when accessors are called?
    ::std::size_t size() const { return (buffer_) ? buffer_->size() : 0u; }
    ::boost::string_view get_view() const
    {
        if (!buffer_) { throw ::std::runtime_error("empty basic_shared_buffer"); }
        return ::boost::string_view(&(*buffer_)[0], buffer_->size());
    }

    ::boost::string_view get_view(::std::size_t pos, ::std::size_t len) const
    {
        if (!buffer_) { throw ::std::runtime_error("empty basic_shared_buffer"); }
        return ::boost::string_view(&(*buffer_)[0] + pos, len);
    }

private:
    using buffer_t = ::std::vector<CharT>;
    ::std::shared_ptr<buffer_t> buffer_;
};

typedef basic_shared_buffer<char> shared_buffer;
typedef basic_shared_buffer<wchar_t> shared_wbuffer;

}   //namespace util
}   //namespace els

#endif //EMAIL_SHARED_BUFFER_HPP
