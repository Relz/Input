#include "Input.h"
#include "gtest/gtest.h"

using namespace std;

TEST(emplace_back_element, returns_true_and_emplaces_back_element)
{
	int const zero = 0;
	int const one = 1;
	int const two = 2;

	vector<int> vect { };

	EXPECT_TRUE(Input::EmplaceBackElement(vect, zero));
	EXPECT_EQ(vect, vector<int>({ 0 }));

	EXPECT_TRUE(Input::EmplaceBackElement(vect, one));
	EXPECT_EQ(vect, vector<int>({ 0, 1 }));

	EXPECT_TRUE(Input::EmplaceBackElement(vect, two));
	EXPECT_EQ(vect, vector<int>({ 0, 1, 2 }));

}
