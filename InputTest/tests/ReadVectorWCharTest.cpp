#include "../../Input/Input.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

using namespace std;

TEST(read_vector_wchar_function, returns_false_if_can_not_read_any_element)
{
	stringstream is("0");
	CInput input(is);
	int arg0;
	vector<char> wChars;
	input.ReadArguments(arg0);
	EXPECT_FALSE(input.ReadVector(wChars));
}

TEST(read_vector_wchar_function, reads_by_wchar_and_push_back_by_default)
{
	vector<char> expectedVector = { '0', ' ', '1', ' ', '2', '3' };
	stringstream is("0 1 23");
	CInput input(is);
	vector<char> wChars;
	EXPECT_TRUE(input.ReadVector(wChars));
	EXPECT_EQ(wChars, expectedVector);
}

TEST(read_vector_wchar_function, can_push_front)
{
	vector<char> expectedVector = { '3', '2', '1', '0' };
	stringstream is("0123");
	CInput input(is);
	vector<char> wChars;
	EXPECT_TRUE(
		input.ReadVector(
			wChars,
			{
				vector<char>(),
				ReadVectorMethod::PUSH_FRONT,
				ReadLimit::UNLIMITED,
				NOT_A_CHARACTER,
				unordered_map<char, char>()
			}
		)
	);
	EXPECT_EQ(wChars, expectedVector);
}

TEST(read_vector_wchar_function, do_not_skip_whitespaces_and_empty_lines_by_default)
{
	vector<char> expectedVector = {};
	stringstream is("\n     \n\n\n             .#.#..#");
	CInput input(is);
	vector<char> wChars;
	EXPECT_FALSE(input.ReadVector(wChars));
	EXPECT_EQ(wChars, expectedVector);
}

TEST(read_vector_wchar_function, does_not_beginning_with_skipping)
{
	vector<char> expectedVector = { ' ', '.', '#', '.', '#', '.', '.', '#' };
	stringstream is("     \n\n\n             .#.#..#");
	CInput input(is);
	vector<char> wChars;
	EXPECT_TRUE(
		input.ReadVector(
			wChars,
			{
				{ ' ', '\n' },
				ReadVectorMethod::PUSH_BACK,
				ReadLimit::UNLIMITED,
				NOT_A_CHARACTER,
				unordered_map<char, char>()
			}
		)
	);
	EXPECT_EQ(wChars, expectedVector);
}

TEST(read_vector_wchar_function, can_skip_whitespaces_and_empty_lines)
{
	vector<char> expectedVector = { '.', '#', '.', '#', '.', '.', '#' };
	stringstream is(".#     \n\n\n             .#..#");
	CInput input(is);
	vector<char> wChars;
	EXPECT_TRUE(
		input.ReadVector(
			wChars,
			{
				{ ' ', '\n' },
				ReadVectorMethod::PUSH_BACK,
				ReadLimit::UNLIMITED,
				NOT_A_CHARACTER,
				unordered_map<char, char>()
			}
		)
	);
	EXPECT_EQ(wChars, expectedVector);
}

TEST(read_vector_wchar_function, can_have_limit)
{
	vector<char> expectedVector = { '.', '#', '.' };
	stringstream is(".#.#..#");
	CInput input(is);
	vector<char> wChars;
	EXPECT_TRUE(
		input.ReadVector(
			wChars,
			{
				vector<char>(),
				ReadVectorMethod::PUSH_BACK,
				3,
				NOT_A_CHARACTER,
				unordered_map<char, char>()
			}
		)
	);
	EXPECT_EQ(wChars, expectedVector);
}
