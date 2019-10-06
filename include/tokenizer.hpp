#pragma once

#include "stream_info.hpp"
#include "token.hpp"

namespace cntlang
{
	token next_token(stream_info& stream);
}
