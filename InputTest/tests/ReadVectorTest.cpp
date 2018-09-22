#include "Input.h"
#include "TestHelper.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(read_vector, returns_false_and_does_not_modify_vector_if_can_not_read_any_element)
{
	stringstream stringStream("0");
	Input input(stringStream);

	vector<int> numbers { 1 };
	vector<int> expectedNumbers { 1 };
	EXPECT_TRUE(input.SkipArgument<char>());
	EXPECT_FALSE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedNumbers);
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 2, false, 0, false, true));
}

TEST(read_vector, returns_false_and_does_not_modify_vector_if_stream_can_not_convert_input_element)
{
	stringstream stringStream("string");
	Input input(stringStream);

	vector<int> numbers { 1 };
	vector<int> expectedNumbers { 1 };
	EXPECT_FALSE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedNumbers);
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, 's', false, false));
}

TEST(read_vector, reads_until_unexpected_type_element)
{
	stringstream stringStream("0 1 2 3 string");
	Input input(stringStream);

	vector<int> numbers;
	vector<int> expectedNumbers { 0, 1, 2, 3 };
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedNumbers);
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 9, true, 's', false, false));
}

TEST(read_vector, skips_whitespaces_if_non_character_type)
{
	stringstream stringStream("     0 1 2 3");
	Input input(stringStream);

	vector<int> numbers;
	vector<int> expectedNumbers { 0, 1, 2, 3 };
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedNumbers);
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 13, false, 0, false, true));
}

TEST(read_vector, does_not_skip_whitespaces_if_character_type)
{
	stringstream stringStream("     0 1 23");
	Input input(stringStream);

	vector<char> characters;
	vector<char> expectedCharacters { ' ', ' ', ' ', ' ', ' ', '0', ' ', '1', ' ', '2', '3' };
	EXPECT_TRUE(input.ReadVector(characters));
	EXPECT_EQ(characters, expectedCharacters);
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 12, false, 0, false, true));
}

TEST(read_vector, does_not_read_end_of_lines_by_default)
{
	stringstream stringStream("\n                             \n\n\n             0 1 2 3");
	Input input(stringStream);

	vector<int> numbers;
	vector<int> expectedNumbers;
	EXPECT_FALSE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedNumbers);
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 1, true, '\n', true, false));
}

TEST(read_vector, can_read_end_of_lines)
{
	{
		stringstream stringStream("\n                             \n\n\n             0 1 2 3");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers { 0, 1, 2, 3 };
		EXPECT_TRUE(input.ReadVector(numbers, true));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 5, 21, false, 0, false, true));
	}
	{
		stringstream stringStream("\r                             \r\r\r             0 1 2 3");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers { 0, 1, 2, 3 };
		EXPECT_TRUE(input.ReadVector(numbers, true));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 5, 21, false, 0, false, true));
	}
	{
		stringstream stringStream("\r\n                             \r\n\r\n\r\n             0 1 2 3");
		Input input(stringStream);

		vector<int> numbers;
		vector<int> expectedNumbers { 0, 1, 2, 3 };
		EXPECT_TRUE(input.ReadVector(numbers, true));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 5, 21, false, 0, false, true));
	}
}

TEST(read_vector, emplaces_back_read_elements_by_default)
{
	stringstream stringStream("0 1 2 3");
	Input input(stringStream);

	vector<int> numbers { 4 };
	vector<int> expectedNumbers { 4, 0, 1, 2, 3 };
	EXPECT_TRUE(input.ReadVector(numbers));
	EXPECT_EQ(numbers, expectedNumbers);
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, false, 0, false, true));
}

TEST(read_vector, reads_strings_splitting_by_spaces)
{
	stringstream stringStream(" 1   2   3   4   5   6   7   8   9\n1 2");
	Input input(stringStream);

	vector<string> strings;
	vector<string> expectedVector { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	EXPECT_TRUE(input.ReadVector(strings));
	EXPECT_EQ(strings, expectedVector);
	EXPECT_TRUE(TestHelper::CheckState(input, 1, 35, true, '\n', true, false));
}

TEST(read_vector, can_specify_custom_for_each_read_element_behavior)
{
	{
		stringstream stringStream("0 1 2 3");
		Input input(stringStream);

		vector<int> numbers { };
		vector<int> expectedNumbers { 3, 2, 1, 0 };

		function<bool(vector<int> &, int &)> emplaceFrontReadElement = [](vector<int> & vect, int & element)
		{
			vect.insert(vect.begin(), element);
			return true;
		};

		EXPECT_TRUE(input.ReadVector(numbers, false, emplaceFrontReadElement));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 8, false, 0, false, true));
	}
	{
		stringstream stringStream("0 1 2 3");
		Input input(stringStream);

		vector<int> numbers { };
		vector<int> expectedNumbers { 0, 1 };

		function<bool(vector<int> &, int &)> emplaceBackTwoReadElement = [](vector<int> & vect, int & element)
		{
			vect.emplace_back(element);
			return vect.size() < 2;
		};

		EXPECT_TRUE(input.ReadVector(numbers, false, emplaceBackTwoReadElement));
		EXPECT_EQ(numbers, expectedNumbers);
		EXPECT_TRUE(TestHelper::CheckState(input, 1, 4, true, ' ', false, false));
	}
}
