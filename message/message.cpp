#include "stdafx.h"

#include "message.hpp"

//using namespace els::email;

/*==================================message===================================*/
els::email::message::message(::std::string content)
: buffer_(content)
{
    e_.swap(::std::make_shared<entity>(boost::string_view(buffer_)));
}

els::email::message::message(els::email::header header, els::email::body body)
	: e_(::std::make_shared<els::email::entity>(header, body))
{
	//complete; buffer_ is not used
}

/*===================================entity===================================*/
els::email::entity::entity(::boost::string_view content, ::std::string buffer)
: buffer_(buffer)
{
	static const char const * BLANK_LINE = "\r\n\r\n";
	auto blank = content.find(BLANK_LINE);
	if (blank != content.npos) { blank += 2u; }	//include first \r\n
	h_ = els::email::header(content.substr(0u, blank), buffer_);
	if (blank != content.npos)
	{
		b_ = els::email::body(content.substr(blank + 2u), buffer_);	//skip leading \r\n in remainder
	}	//if
}

/*===================================field====================================*/

/*===================================header===================================*/
els::email::header::header()
{
	//complete
}

els::email::header::header(::boost::string_view content, ::std::string buffer)
{
}

/*====================================body====================================*/
