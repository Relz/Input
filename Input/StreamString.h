#ifndef PROJECT_STREAMSTRING_H
#define PROJECT_STREAMSTRING_H

#include <string>
#include "../StreamPosition/StreamPosition.h"

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
