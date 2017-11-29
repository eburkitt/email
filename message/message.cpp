#include "stdafx.h"

#include "message.hpp"

using namespace els::email;

/*==================================message===================================*/
els::email::message::message(::std::string content)
: buffer_(content)
{
    e_.swap(::std::make_shared<entity>(boost::string_view(buffer_)));
}

/*===================================entity===================================*/
els::email::entity::entity(::boost::string_view content)
{
	static const char const * BLANK_LINE = "\r\n\r\n";
	auto blank = content.find(BLANK_LINE);
	if (blank != content.npos) { blank += 2L; }	//include first \r\n
	h_ = class header(content.substr(0L, blank));
	if (blank != content.npos)
	{
		b_ = class body(content.substr(blank + 2));	//skip leading \r\n in remainder
	}	//if
}

/*===================================field====================================*/

/*===================================header===================================*/
els::email::header::header()
{
	//complete
}

els::email::header::header(::boost::string_view content)
{
}

/*====================================body====================================*/
