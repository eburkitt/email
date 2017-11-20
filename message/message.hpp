#pragma once

#include <vector>
#include <string>

#include <boost/utility/string_view.hpp> 

namespace els {
namespace email {

class header;
class entity;
class body;

//an RFC2822 et seq. email message
class message
{
public:
    //ctor for existing off-the-wire message
	message(::std::string content);	//assumed to be valid email message; throws if it isn't
    //ctor for creating message from parts
    message(header header, body body);  //

	//shortcut access to popular header fields
    ::boost::string_view to() const;
    ::boost::string_view from() const;
    ::boost::string_view subject() const;

	header const &header() const { return e_.header(); }
	body const &body() const { return e_.body(); }

private:
	entity e_;
	::std::string buffer_;
};

//email message, as well as body component of a multi-part message
class entity
{
public:
    entity(header header, body body);
    entity(header header);
    entity(body body);

    header const &header() const { return h_; }
    body const &body() const { return b_; }

private:
	header h_;
	body b_;
    ::std::string buffer_;	//used only if instance created outside of a header
};

//header field
class field
{
public:
    ::boost::string_view name() const;
    ::boost::string_view value() const;

private:
	::boost::string_view name_;
	::boost::string_view value_;
	::std::string buffer_;	//used only if instance created outside of a header
};

//collection of header fields
class header
{
public:
    //default ctor; fields added via operator[]
    header();
    //TODO: ctor taking fields, or vector or map of name/values
    //view ctor
    header(::boost::string_view content);

    ::std::string operator[](::std::string name);

private:
	::std::vector<field> fields_;
};

//collection of multi-part message parts, or body of non-multi-part message
//(implementation of latter TBD)
class body
{
public:
    //view ctor
	body(::boost::string_view content);
    //TODO: how to construct from pieces

private:
	::std::vector<entity> content_;
};

}	//namespace email
}	//namespace els
