#pragma once

#include <vector>
#include <string>
#include <memory>

#include <boost/utility/string_view.hpp> 

#include "shared_buffer.hpp"

namespace els {
namespace email {

enum content_transfer_encoding
{
    cte_7bit, cte_qprintable, cte_base64
};

using octets_t = ::std::vector<unsigned char>;

class entity;
class header;
class body;

//an RFC2822 et seq. email message
class message
{
public:
    //ctor for existing off-the-wire message
    explicit message(::std::string content);    //assumed to be valid email message; throws if it isn't
    //ctor for creating message from parts
    message(header header, body body);

	//shortcut access to popular header fields
    ::boost::string_view to() const;
    ::boost::string_view from() const;
    ::boost::string_view subject() const;

	header const &header() const { return e_->header(); }
	body const &body() const { return e_->body(); }

private:
	::std::shared_ptr<entity> e_;
	::els::util::shared_buffer buffer_;
};

//email message, as well as body component of a multi-part message
class entity
{
public:
    //view ctor
    entity(::boost::string_view content, ::els::util::shared_buffer buffer);
    entity(header header, body body);
    explicit entity(header header);
    explicit entity(body body);

    header const &header() const { return h_; }
    body const &body() const { return b_; }

private:
	class header h_;
	class body b_;
    ::els::util::shared_buffer buffer_;
};

//header field
class field
{
public:
	//ctors??
    ::boost::string_view name() const;
    ::boost::string_view value() const;

private:
	::boost::string_view name_;
	::boost::string_view value_;
	::std::string buffer_;
};

//collection of header fields
class header
{
public:
	using fields_t = ::std::vector<field>;
    //default ctor; produces empty header
    header();
    //TODO: ctor taking fields, or vector or map of name/values
    //view ctor
    header(::boost::string_view content, ::els::util::shared_buffer buffer);

	//return single named field; first if there are multiple w/ same name
	::std::string operator[](char const *name) const;
	::std::string operator[](::std::string name) const;
	::std::string operator[](::boost::string_view name) const;

	class field field(char const *name) const;
	class field field(::std::string name) const;
	class field field(boost::string_view name) const;
	//return collection of all named fields
	fields_t fields(char const *name) const;
    fields_t fields(::std::string name) const;
    fields_t fields(boost::string_view name) const;

private:
    using fields_p_t = ::std::shared_ptr<fields_t>;
	fields_p_t fields_;
};

//collection of multi-part message parts, or body of non-multi-part message
//(implementation of latter TBD)
class body
{
public:
    using entities_t = ::std::vector<entity>;
    //default ctor; produces empty body
    body();
    //view ctor
	body(::boost::string_view content, ::els::util::shared_buffer buffer);
    //from-content ctor
    explicit body(::std::string content);

    //construct specific body types from supplied data
    static body octet_stream(octets_t data, content_transfer_encoding cte);
    //text() will always use subtype "plain"
    static body text(::std::string data /*,char encoding, content transfer encoding*/);
    //all following will be base64 encoded
    static body image(octets_t data, ::std::string subtype);
    static body audio(octets_t data, ::std::string subtype);
    static body video(octets_t data, ::std::string subtype);
    static body application(octets_t data, ::std::string subtype);

private:
    using entities_p_t = ::std::shared_ptr<entities_t>;
	entities_p_t content_;
    ::els::util::shared_buffer buffer_;
};

}	//namespace email
}	//namespace els
