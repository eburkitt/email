#include "stdafx.h"

#include "message.hpp"

//using namespace els::email;

/*==================================message===================================*/
els::email::message::message(::std::string const &content)
{
    ::els::util::shared_buffer b(content);
	e_ = ::std::move(entity(b.get_view(), ::std::move(b)));
}

els::email::message::message(header header, body body)
: e_(::std::move(header), ::std::move(body))
{
	//complete
}

/*===================================entity===================================*/
els::email::entity::entity(::boost::string_view const &content, ::els::util::shared_buffer buffer)
: buffer_(std::move(buffer))
{
	static const char const * BLANK_LINE = "\r\n\r\n";
	auto blank = content.find(BLANK_LINE);
	if (blank != content.npos) { blank += 2u; }	//include first \r\n
	h_ = els::email::header(content.substr(0u, blank), buffer_);
	if (blank != content.npos)
	{
		auto content_type = h_.field("content-type");
//		if (content_type) URHERE - check content_type for existence, and for "subvalues"
//		of "multipart/mixed" and "boundary=xx". If found, call body ctor taking boundary,
//		passing xx
        //skip leading \r\n in remainder
		b_ = els::email::body(content.substr(blank + 2u), buffer_);
	}	//if
}

els::email::entity::entity(header header, body body)
: h_(::std::move(header))
, b_(::std::move(body))
{
	//complete; buffer_ is not used
}

els::email::entity::entity(header header)
: h_(::std::move(header))
{
	//complete; b_ is empty, buffer_ is not used
}

els::email::entity::entity(body body)
: b_(::std::move(body))
{
	//complete; h_ is empty, buffer_ is not used
}

/*===================================field====================================*/
els::email::field::field(char const *name)
{
	buffer_ = ::std::move(::els::utils::shared_buffer(name, ::std::strlen(name)));
	name_ = ::std::move(buffer_.get_view());
}

els::email::field::field(::std::string const &name)
{
	buffer_ = ::std::move(::els::utils::shared_buffer(name));
	name_ = ::std::move(buffer_.get_view());
}

els::email::field::field(::boost::string_view const &name)
{
	buffer_ = ::std::move(::els::utils::shared_buffer(name));
	name_ = ::std::move(buffer_.get_view());
}

els::email::field::field(::boost::string_view name, ::boost::string_view value,
                         ::els::util::shared_buffer buffer)
: name_(::std::move(name))
, value_(::std::move(value))
, buffer_(::std::move(buffer))
{
    //complete
}

::boost::string_view els::email::field::name() const
{
    return name_;
}

::boost::string_view els::email::field::value() const
{
    return value_;
}

bool els::email::field::empty() const
{
    return value_.empty();
}

/*===================================header===================================*/
els::email::header::header()
{
	//complete
}

els::email::header::header(::boost::string_view const &content, ::els::util::shared_buffer const &buffer)
{
}

/*====================================body====================================*/
els::email::body::body(::boost::string_view const &content, ::els::util::shared_buffer buffer)
: buffer_(::std::move(buffer))
{

}
