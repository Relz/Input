#ifndef INPUTLIBRARYTEST_TESTHELPER_H
#define INPUTLIBRARYTEST_TESTHELPER_H

#include "gtest/gtest.h"

class TestHelper
{
public:
	static bool CheckState(
		Input const & input,
		long expectedLine,
		long expectedColumn,
		bool isNextCharacterExpected,
		char expectedNextCharacter,
		bool expectedIsEndOfLine,
		bool expectedIsEndOfStream)
	{
		StreamPosition position = input.GetPosition();

		char nextCharacter;

		EXPECT_EQ(position.GetLine(), expectedLine);
		EXPECT_EQ(position.GetColumn(), expectedColumn);
		bool isNextCharacterExists = false;
		if (isNextCharacterExpected)
		{
			isNextCharacterExists = input.GetNextCharacter(nextCharacter);
			EXPECT_TRUE(isNextCharacterExists);
			EXPECT_EQ(nextCharacter, expectedNextCharacter);
		}
		EXPECT_EQ(input.IsEndOfLine(), expectedIsEndOfLine);
		EXPECT_EQ(input.IsEndOfStream(), expectedIsEndOfStream);

		return position.GetLine() == expectedLine
			&& position.GetColumn() == expectedColumn
			&& (!isNextCharacterExpected || (isNextCharacterExists && nextCharacter == expectedNextCharacter))
			&& input.IsEndOfLine() == expectedIsEndOfLine
			&& input.IsEndOfStream() == expectedIsEndOfStream;
	}
};

#endif
