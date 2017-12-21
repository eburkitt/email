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
    explicit message(::std::string const &content);    //assumed to be valid email message; throws if it isn't
    //ctor for creating message from parts
    message(header header, body body);

	//shortcut access to popular header fields
    ::boost::string_view to() const;
    ::boost::string_view from() const;
    ::boost::string_view subject() const;

	header const &header() const { return e_.header(); }
	body const &body() const { return e_.body(); }

private:
	entity e_;
};

//email message, as well as body component of a multi-part message
class entity
{
public:
	//default ctor; produces empty entity, suitable for assignment to
	entity();
    //view ctor
    entity(::boost::string_view const &content, ::els::util::shared_buffer buffer);
    entity(header header, body body);
    explicit entity(header header);	//just a header; no body
    explicit entity(body body);		//just a body; no header

    header const &header() const { return h_; }
    body const &body() const { return b_; }

	//construct specific entity types from supplied data
	static entity octet_stream(octets_t const &data, content_transfer_encoding cte);
	//text() will always use subtype "plain"
	static entity text(::std::string const &data /*,char encoding, content transfer encoding*/);
	//all following will be base64 encoded
	static entity image(octets_t const &data, ::std::string const &subtype);
	static entity audio(octets_t const &data, ::std::string const &subtype);
	static entity video(octets_t const &data, ::std::string const &subtype);
	static entity application(octets_t const &data, ::std::string const &subtype);

	//Note to dumbass:
	//?? to_string() const <- return stringish thing made from buffer_
	//if buffer_ is !empty, otherwise made from h_.to_string() +
	//b_.to_string()

private:

	class header h_;
	class body b_;
    ::els::util::shared_buffer buffer_;
};

//header field
class field
{
public:
    //view ctors
	//TODO: what about null char const* param?
    explicit field(char const *name);			//only name param == empty field
	explicit field(::std::string const &name);
	explicit field(::boost::string_view const &name);
    field(::boost::string_view name, ::boost::string_view value,
          ::els::util::shared_buffer buffer);

    ::boost::string_view name() const;
    ::boost::string_view value() const;

    bool empty() const;

private:
	::boost::string_view name_;
	::boost::string_view value_;
    ::els::util::shared_buffer buffer_;
};

//collection of header fields
//Re adding fields to header: consider keeping added fields in a separate collection
//and, in header.to_string(), calling their to_string() members before incorporating buffer_.
//This idea requires header to keep its own copy of buffer it receives in ctor(s) with
//a shared_buffer param. This will prepend added headers to existing ones, which is, I
//think, recommended practice
class header
{
public:
	using fields_t = ::std::vector<field>;
    //default ctor; produces empty header, suitable for assignment to
    header();
    //TODO: ctor taking fields, or vector or map of name/values
    //view ctor
    header(::boost::string_view const &content, ::els::util::shared_buffer const &buffer);

	//return single named field's value, unfolded; first if there are multiple w/ same name
	//TODO: what about null char const* param?
	::std::string operator[](char const *name) const;
	::std::string operator[](::std::string const &name) const;
	::std::string operator[](::boost::string_view const &name) const;

	//return single named field with a raw value; first if there are multiple w/ same name
	//TODO: what about null char const* param?
	class field field(char const *name) const;
	class field field(::std::string const &name) const;
	class field field(::boost::string_view const &name) const;

	//return collection of all fields named 'name'
	//TODO: what about null char const* param?
	fields_t fields(char const *name) const;
    fields_t fields(::std::string const &name) const;
    fields_t fields(::boost::string_view const &name) const;

private:
    using fields_p_t = ::std::shared_ptr<fields_t>;
	fields_p_t fields_;
    ::els::util::shared_buffer buffer_;
};

//collection of multi-part message parts, or body of non-multi-part message
//(implementation of latter TBD)
class body
{
public:
    using entities_t = ::std::vector<entity>;
    //default ctor; produces empty body, suitable for assignment to
    body();
    //view ctor
	body(::boost::string_view const &content, ::els::util::shared_buffer buffer);
    //from-content ctor
    explicit body(::std::string const &content);

private:
    using entities_p_t = ::std::shared_ptr<entities_t>;
	entities_p_t content_;
    ::els::util::shared_buffer buffer_;
};

}	//namespace email
}	//namespace els
