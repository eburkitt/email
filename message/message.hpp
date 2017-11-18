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
	message(::std::string content);	//assumed to be valid email message; throws if it isn't

	//shortcut access to popular header fields
	::std::string to() const;
	::std::string from() const;
	::std::string subject() const;

	header header() const;
	body body() const;

private:
	entity e_;
	::std::string buffer_;
};

//email message, as well as body component of a multi-part message
class entity
{
public:
	entity(::boost::string_view content);

private:
	header h_;
	body b_;
	::boost::string_view buf_view_;	//one or the other of this and buffer_, not both
	::std::string buffer_;
};

//header field
class field
{
	::std::string name() const;
	::std::string value() const;
private:
	::boost::string_view name_;
	::boost::string_view value_;
	::std::string buffer_;	//used only if created outside of a header
};

//collection of header fields
class header
{

private:
	::std::vector<field> fields_;
};

//collection of multi-part message parts, or body of non-multi-part message
//(implementation of latter TBD)
class body
{
public:
	body(::boost::string_view content, ::boost::string_view content_type);

private:
	::std::vector<entity> content_;
};

}	//namespace email
}	//namespace els
