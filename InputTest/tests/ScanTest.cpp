#include "../../Input/Input.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(scan_function, returns_false_if_end_of_stream_scanned_otherwise_string_and_delimiter)
{
	istringstream is("class User");
	CInput input(is);
	{
		string scannedString;
		StreamPosition scannedStringPosition;
		string delimiter;
		StreamPosition delimiterPosition;
		EXPECT_TRUE(input.Scan({ " " }, scannedString, scannedStringPosition, delimiter, delimiterPosition));
		EXPECT_EQ(scannedString, "class");
		EXPECT_EQ(delimiter, " ");
		EXPECT_EQ(scannedStringPosition.GetLine(), 1);
		EXPECT_EQ(scannedStringPosition.GetColumn(), 1);
		EXPECT_EQ(delimiterPosition.GetLine(), 1);
		EXPECT_EQ(delimiterPosition.GetColumn(), 6);
	}
	{
		string scannedString;
		StreamPosition scannedStringPosition;
		string delimiter;
		StreamPosition delimiterPosition;
		EXPECT_TRUE(input.Scan({ " " }, scannedString, scannedStringPosition, delimiter, delimiterPosition));
		EXPECT_EQ(scannedString, "User");
		EXPECT_EQ(delimiter, "");
		EXPECT_EQ(scannedStringPosition.GetLine(), 1);
		EXPECT_EQ(scannedStringPosition.GetColumn(), 7);
		EXPECT_EQ(delimiterPosition.GetLine(), 1);
		EXPECT_EQ(delimiterPosition.GetColumn(), 7);
	}
	{
		string scannedString;
		StreamPosition scannedStringPosition;
		string delimiter;
		StreamPosition delimiterPosition;
		EXPECT_FALSE(input.Scan({ " " }, scannedString, scannedStringPosition, delimiter, delimiterPosition));
		EXPECT_EQ(scannedString, "");
		EXPECT_EQ(delimiter, "");
		EXPECT_EQ(scannedStringPosition.GetLine(), 1);
		EXPECT_EQ(scannedStringPosition.GetColumn(), 1);
		EXPECT_EQ(delimiterPosition.GetLine(), 1);
		EXPECT_EQ(delimiterPosition.GetColumn(), 1);
	}
}

TEST(scan_function, can_work_with_multiple_lines_and_delimiters)
{
	vector<string> expectedScannedStrings { "class", "User", "", "Constructor", "String", "name", "", "", "", "", "" };
	vector<string> expectedScannedDelimiters { " ", "{", "\t", "(", " ", ")", "\t", "{", " ", "}", "}" };
	istringstream is("class User\n{\n\tConstructor(String name)\n\t{ }\n}");
	CInput input(is);
	vector<string> scannedStrings;
	vector<string> scannedDelimiters;
	string scannedString;
	StreamPosition scannedStringPosition;
	string delimiter;
	StreamPosition delimiterPosition;
	vector<string> delimiters = { " ", "(", ")", "{", "}", ";", "\t" };
	while (input.Scan(delimiters, scannedString, scannedStringPosition, delimiter, delimiterPosition))
	{
		scannedStrings.push_back(scannedString);
		scannedDelimiters.push_back(delimiter);
	}
	EXPECT_EQ(scannedStrings, expectedScannedStrings);
	EXPECT_EQ(scannedDelimiters, expectedScannedDelimiters);
}

TEST(scan_function, can_work_with_big_delimiter)
{
	vector<string> expectedScannedStrings = { "user", "name", "user->name", "user-", "name" };
	vector<string> expectedScannedDelimiters = { "-->", " ", " ", "-->", "" };
	istringstream is("user-->name user->name user--->name");
	CInput input(is);
	vector<string> scannedStrings;
	vector<string> scannedDelimiters;
	string scannedString;
	StreamPosition scannedStringPosition;
	string delimiter;
	StreamPosition delimiterPosition;
	while (input.Scan({ " ", "-->" }, scannedString, scannedStringPosition, delimiter, delimiterPosition))
	{
		scannedStrings.push_back(scannedString);
		scannedDelimiters.push_back(delimiter);
		delimiter.clear();
	}
	EXPECT_EQ(scannedStrings, expectedScannedStrings);
	EXPECT_EQ(scannedDelimiters, expectedScannedDelimiters);
}
