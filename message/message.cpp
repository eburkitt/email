#include "stdafx.h"

#include "message.hpp"

using namespace els::email;

els::email::message::message(::std::string content)
: buffer_(content)

{
    e_.swap(::std::make_shared(entity(buffer_)));
}
