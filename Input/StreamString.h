#ifndef PROJECT_STREAMSTRING_H
#define PROJECT_STREAMSTRING_H

#include "../StreamPosition/StreamPosition.h"
#include <string>

class StreamString
{
public:
	StreamString() = default;
	StreamString(std::string str, StreamPosition position)
		: string(std::move(str))
		, position(position)
	{}

	std::string string;
	StreamPosition position;
};

#endif //PROJECT_STREAMSTRING_H
