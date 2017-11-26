#include "stdafx.h"

#include "message.hpp"

els::email::message::message(::std::string content)
: buffer_(content)
, e_(::boost::string_view(content))
{
    //complete
}
