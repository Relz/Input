#include "Input.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace std;

TEST(scan, returns_false_if_end_of_stream_scanned_otherwise_string_and_delimiter)
{
	istringstream is("class User");
	Input input(is);
	{
		StreamString scannedStreamString;
		StreamString delimiterStreamString;
		EXPECT_TRUE(input.Scan({" "}, scannedStreamString, delimiterStreamString));
		EXPECT_EQ(scannedStreamString.string, "class");
		EXPECT_EQ(delimiterStreamString.string, " ");
		EXPECT_EQ(scannedStreamString.position.GetLine(), 1);
		EXPECT_EQ(scannedStreamString.position.GetColumn(), 1);
		EXPECT_EQ(delimiterStreamString.position.GetLine(), 1);
		EXPECT_EQ(delimiterStreamString.position.GetColumn(), 6);
	}
	{
		StreamString scannedStreamString;
		StreamString delimiterStreamString;
		EXPECT_TRUE(input.Scan({" "}, scannedStreamString, delimiterStreamString));
		EXPECT_EQ(scannedStreamString.string, "User");
		EXPECT_EQ(delimiterStreamString.string, "");
		EXPECT_EQ(scannedStreamString.position.GetLine(), 1);
		EXPECT_EQ(scannedStreamString.position.GetColumn(), 7);
		EXPECT_EQ(delimiterStreamString.position.GetLine(), 1);
		EXPECT_EQ(delimiterStreamString.position.GetColumn(), 1);
	}
	{
		StreamString scannedStreamString;
		StreamString delimiterStreamString;
		EXPECT_FALSE(input.Scan({" "}, scannedStreamString, delimiterStreamString));
		EXPECT_EQ(scannedStreamString.string, "");
		EXPECT_EQ(delimiterStreamString.string, "");
		EXPECT_EQ(scannedStreamString.position.GetLine(), 1);
		EXPECT_EQ(scannedStreamString.position.GetColumn(), 1);
		EXPECT_EQ(delimiterStreamString.position.GetLine(), 1);
		EXPECT_EQ(delimiterStreamString.position.GetColumn(), 1);
	}
}

TEST(scan, can_work_with_multiple_lines_and_delimiters)
{
	vector<string> expectedScannedStrings {"class", "User", "", "", "", "Constructor", "String", "name", "", "", "", "", "", "", ""};
	vector<string> expectedScannedDelimiters {" ", "\n", "{", "\n", "\t", "(", " ", ")", "\n", "\t", "{", " ", "}", "\n", "}"};
	istringstream is("class User\n{\n\tConstructor(String name)\n\t{ }\n}");
	Input input(is);
	vector<string> scannedStrings;
	vector<string> scannedDelimiters;
	StreamString scannedStreamString;
	StreamString delimiterStreamString;
	vector<string> delimiters {" ", "(", ")", "{", "}", ";", "\t", "\n"};
	while (input.Scan(delimiters, scannedStreamString, delimiterStreamString))
	{
		scannedStrings.push_back(scannedStreamString.string);
		scannedDelimiters.push_back(delimiterStreamString.string);
	}
	EXPECT_EQ(scannedStrings, expectedScannedStrings);
	EXPECT_EQ(scannedDelimiters, expectedScannedDelimiters);
}

TEST(scan, can_work_with_big_delimiter)
{
	vector<string> expectedScannedStrings {"user", "name", "user->name", "user-", "name"};
	vector<string> expectedScannedDelimiters {"-->", " ", " ", "-->", ""};
	istringstream is("user-->name user->name user--->name");
	Input input(is);
	vector<string> scannedStrings;
	vector<string> scannedDelimiters;
	StreamString scannedStreamString;
	StreamString delimiterStreamString;
	while (input.Scan({" ", "-->"}, scannedStreamString, delimiterStreamString))
	{
		scannedStrings.push_back(scannedStreamString.string);
		scannedDelimiters.push_back(delimiterStreamString.string);
		delimiterStreamString.string.clear();
	}
	EXPECT_EQ(scannedStrings, expectedScannedStrings);
	EXPECT_EQ(scannedDelimiters, expectedScannedDelimiters);
}
