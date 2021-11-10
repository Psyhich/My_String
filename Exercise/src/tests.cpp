#include <gtest/gtest.h>

#include "my_string.h"
class CMyStringFixture : public ::testing::Test
{
protected:
	Exercise_1::CMyString myString;
	const size_t sTestDataLength{17};
	const char szTestData[17]{"Some test string"};

public:
	void SetUp() override
	{
		myString = Exercise_1::CMyString(szTestData);
	}
};

TEST_F(CMyStringFixture, DataCorrectnesTest) 
{

	ASSERT_EQ(sTestDataLength, myString.size());

	ASSERT_STREQ(szTestData, myString.data());

	ASSERT_EQ(szTestData[sTestDataLength - 1], '\0');
}

TEST_F(CMyStringFixture, DataCopyTest)
{
	Exercise_1::CMyString copiedString{myString};

	ASSERT_EQ(myString.size(), copiedString.size());

	ASSERT_STREQ(myString.data(), copiedString.data());

	Exercise_1::CMyString operatorCopiedString{myString};

	ASSERT_EQ(myString.size(), operatorCopiedString.size());

	ASSERT_STREQ(myString.data(), operatorCopiedString.data());
}

