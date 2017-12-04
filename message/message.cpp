#include "stdafx.h"

#include "message.hpp"

//using namespace els::email;

/*==================================message===================================*/
els::email::message::message(::std::string content)
: buffer_(content)
{
    //NB - make_shared() as parameter to swap() makes CLion's editor complain
    //"expression must be an lvalue"
    auto x = ::std::make_shared<entity>(buffer_.get_view(), buffer_);
    e_.swap(x);
}

els::email::message::message(els::email::header header, els::email::body body)
	: e_(::std::make_shared<els::email::entity>(header, body))
{
	//complete; buffer_ is not used
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
        //skip leading \r\n in remainder
		b_ = els::email::body(content.substr(blank + 2u), buffer_);
	}	//if
}

/*===================================field====================================*/

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
{

}
