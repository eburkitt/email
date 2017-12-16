#include "stdafx.h"

#include "message.hpp"

//using namespace els::email;

/*==================================message===================================*/
els::email::message::message(::std::string const &content)
{
    ::els::util::shared_buffer b(content);
    //NB - make_shared() as parameter to swap() makes CLion's editor complain
    //"expression must be an lvalue"
    auto x = ::std::make_shared<entity>(b.get_view(), b);
    e_.swap(x);
}

els::email::message::message(els::email::header header, els::email::body body)
	: e_(::std::make_shared<els::email::entity>(header, body))
{
	//complete
}

/*===================================entity===================================*/
els::email::entity::entity(::boost::string_view content, ::els::util::shared_buffer buffer)
: buffer_(buffer)
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

/*===================================field====================================*/
els::email::field::field(::boost::string_view name, ::boost::string_view value,
                         ::els::util::shared_buffer buffer)
: name_(name)
, value_(value)
, buffer_(buffer)
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

els::email::header::header(::boost::string_view content, ::els::util::shared_buffer buffer)
{
}

/*====================================body====================================*/
els::email::body::body(::boost::string_view content, ::els::util::shared_buffer buffer)
: buffer_(buffer)
{

}
