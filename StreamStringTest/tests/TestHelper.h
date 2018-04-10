#ifndef PROJECT_TESTHELPER_H
#define PROJECT_TESTHELPER_H

class TestHelper
{
public:
	static bool TestStreamString(
		StreamString const & streamString, std::string const & expectedString, StreamPosition const & expectedPosition)
	{
		return streamString.string == expectedString && streamString.position == expectedPosition;
	}
};

#endif // PROJECT_TESTHELPER_H
