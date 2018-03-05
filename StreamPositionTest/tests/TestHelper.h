#ifndef PROJECT_TESTHELPER_H
#define PROJECT_TESTHELPER_H

#include <functional>

static long MIN_LINE = 1;
static long MIN_COLUMN = 1;

static bool TestStreamPosition(
	StreamPosition streamPosition,
	std::function<void(StreamPosition &)> const & function,
	long expectedLine,
	long expectedColumn)
{
	function(streamPosition);
	return streamPosition.GetLine() == expectedLine && streamPosition.GetColumn() == expectedColumn;
}

static void EmptyFunction(StreamPosition & something) {}

#endif // PROJECT_TESTHELPER_H
